#include <visualization_msgs/Marker.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Path.h>

#include "navigation.h"
#include "commands.h"
#include "gridmap.h"

#include "NearestFrontierPlanner.h"

#include <map>
#include <set>

#define FREQUENCY 5.0

using namespace ros;
using namespace tf;
nav_msgs::OccupancyGrid mapMsg;
bool mapMsgflg;

void mapSubscribeCallback(const nav_msgs::OccupancyGrid& grid);

RobotNavigator::RobotNavigator()
{
	NodeHandle robotNode;
    
	mCurrentPlan = NULL;
    NodeHandle navigatorNode;

    mMapSubscriber = navigatorNode.subscribe("map", 100, mapSubscribeCallback);
    mMapPublisher = navigatorNode.advertise<nav_msgs::OccupancyGrid>("mymap", 100);
	mPlanPublisher = navigatorNode.advertise<nav_msgs::Path>("plan", 1);
	mMarkerPublisher = navigatorNode.advertise<visualization_msgs::Marker>("markers", 1, true);

    // Get parameters
	navigatorNode.param("map_inflation_radius", mInflationRadius, 1.0);
	navigatorNode.param("robot_radius", mRobotRadius, 0.3);
	navigatorNode.param("exploration_strategy", mExplorationStrategy, std::string("NearestFrontierPlanner"));
	navigatorNode.param("navigation_goal_distance", mNavigationGoalDistance, 1.0);
	navigatorNode.param("navigation_goal_angle", mNavigationGoalAngle, 1.0);
	navigatorNode.param("exploration_goal_distance", mExplorationGoalDistance, 3.0);
	navigatorNode.param("navigation_homing_distance", mNavigationHomingDistance, 3.0);
	navigatorNode.param("min_replanning_period", mMinReplanningPeriod, 3.0);
	navigatorNode.param("max_replanning_period", mMaxReplanningPeriod, 1.0);

	mCostObstacle = 100;
	mCostLethal = (1.0 - (mRobotRadius / mInflationRadius)) * (double)mCostObstacle;
	
	robotNode.param("map_frame", mMapFrame, std::string("map"));
	robotNode.param("robot_frame", mRobotFrame, std::string("robot"));

	// Apply tf_prefix to all used frame-id's
	mRobotFrame = mTfListener.resolve(mRobotFrame);
	mMapFrame = mTfListener.resolve(mMapFrame);


    mapMsgflg = false; //是否调用了map callback函数标志
    
    mHasNewMap = false;
	mIsStopped = false;
	mIsPaused = false;
	mapInflatedFlg = false;
	mStatus = NAV_ST_IDLE;
	mCellInflationRadius = 0;

}
RobotNavigator::~RobotNavigator()
{
	delete[] mCurrentPlan;
}
void RobotNavigator::exploreGoGoal()
{

	unsigned int cycle = 0;
	unsigned int lastCheck = 0;
	unsigned int recheckCycles = mMinReplanningPeriod * FREQUENCY; //3*5 = 15
	unsigned int recheckThrottle = mMaxReplanningPeriod * FREQUENCY;//1*5 = 5

	NearestFrontierPlanner explorePlan;

    ros::Rate loopRate(FREQUENCY);

    while(true)
    {
		if(!ok())
		{
			ROS_INFO("Exploration has been preempted externally.");
			return;
		}
		
		mHasNewMap = false;   //起什么作用？？
		if(!setCurrentPosition())
		{
			ROS_ERROR("Exploration failed, could not get current position.");
			ROS_ERROR("we have to wait for map update");
			continue;
		}

		cycle++;

		//第一次或者超过了replan的时间限制就要重新规划路径
		bool reCheck = lastCheck == 0 || cycle - lastCheck > recheckCycles;
		bool planOk = mCurrentPlan && mCurrentPlan[mStartPoint] >= 0;
		bool nearGoal = planOk && ((cycle - lastCheck) > recheckThrottle)
							&& mCurrentPlan[mStartPoint] <= mExplorationGoalDistance;
							//mCurrentPlan需要初始化？？
		//只有当第一次进入循环或者超过重新规划路径的时间循环时
		//或者接近目标点并且时间超过最小规划路径间隔时间时才会重新规划路径
		if(reCheck || nearGoal)
		{
			lastCheck = cycle;
			bool success = false;

			//可能出现在inflation之后的图像上再inflation
			if(preparePlan())
			{
				ROS_INFO("preparePlan lastcheck:%d", lastCheck);
				int result = explorePlan.findExplorationTarget(&mCurrentMap, mStartPoint, mGoalPoint);
				
				switch(result)
				{
				case EXPL_TARGET_SET:
					ROS_ERROR("expl_target_set");
					success = createPlan();
					mStatus = NAV_ST_EXPLORING;
					break;
				case EXPL_FINISHED:
					ROS_ERROR("expl_finished");
					// {
					// 	nav2d_navigator::ExploreResult r;
					// 	r.final_pose.x = mCurrentPositionX;
					// 	r.final_pose.y = mCurrentPositionY;
					// 	r.final_pose.theta = mCurrentDirection;
					// 	mExploreActionServer->setSucceeded(r);
					// }
					ROS_INFO("Exploration has finished.");
					return;
				case EXPL_WAITING:
					ROS_ERROR("expl_waiting");
					break;
				case EXPL_FAILED:
					ROS_ERROR("expl_failed");
					break;
				default:
					ROS_ERROR("Exploration planner returned invalid status code: %d!", result);
				}
			}
		}

        spinOnce();
		loopRate.sleep();
        //ROS_INFO("sleep:%d", loopRate.sleep());//有作用
		if(loopRate.cycleTime() > ros::Duration(1.0f / FREQUENCY))
		{
			//从start到sleep的程序运行时间，而不是循环的时间，不包含sleep的时间
			ROS_INFO("cycle tim %.4f seconds", loopRate.cycleTime().toSec());
			
		}
    }
}


typedef std::multimap<double,unsigned int> Queue;
typedef std::pair<double,unsigned int> Entry;


bool RobotNavigator::preparePlan()
{
	// Get the current map
	if(!getMap()) // return false;
	{
		if(mCellInflationRadius == 0) return false;//指还没有初始化
		ROS_WARN("Could not get a new map, trying to go with the old one...");
	}
	
	// Where am I?
	if(!setCurrentPosition()) return false;
	
	//新地图就需要膨胀
	if(!mapInflatedFlg)
	{
		// Clear robot footprint in map
		unsigned int x = 0, y = 0;
		if(mCurrentMap.getCoordinates(x, y, mStartPoint))
			for(int i = -mCellRobotRadius; i < (int)mCellRobotRadius; i++)
				for(int j = -mCellRobotRadius; j < (int)mCellRobotRadius; j++)
					mCurrentMap.setData(x+i, y+j, 0);
				
		mInflationTool.inflateMap(&mCurrentMap);

		//发布膨胀地图
	    mMapPublisher.publish(mCurrentMap.getMap());
		mapInflatedFlg = true;
	}

	return true;
}


bool RobotNavigator::createPlan()
{	
	ROS_INFO("Map-Value of goal point is %d, lethal threshold is %d.", mCurrentMap.getData(mGoalPoint), mCostLethal);
	
	unsigned int goal_x = 0, goal_y = 0;
	if(mCurrentMap.getCoordinates(goal_x,goal_y,mGoalPoint))
	{
		visualization_msgs::Marker marker;
		marker.header.frame_id = "/map";
		marker.header.stamp = ros::Time();
		marker.id = 0;
		marker.type = visualization_msgs::Marker::CYLINDER;
		marker.action = visualization_msgs::Marker::ADD;
		marker.pose.position.x = mCurrentMap.getOriginX() + (((double)goal_x+0.5) * mCurrentMap.getResolution());
		marker.pose.position.y = mCurrentMap.getOriginY() + (((double)goal_y+0.5) * mCurrentMap.getResolution());
		marker.pose.position.z = 0.5;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;
		marker.scale.x = mCurrentMap.getResolution() * 3.0;
		marker.scale.y = mCurrentMap.getResolution() * 3.0;
		marker.scale.z = 1.0;
		marker.color.a = 1.0;
		marker.color.r = 1.0;
		marker.color.g = 0.0;
		marker.color.b = 0.0;
		ROS_ERROR("goal x:%d, y:%d", goal_x, goal_y);
		mMarkerPublisher.publish(marker);
	}else
	{
		ROS_ERROR("Couldn't ressolve goal point coordinates!");
	}


	Queue queue;
	
	// Reset the plan
	int mapSize = mCurrentMap.getSize();
	for(int i = 0; i < mapSize; i++)
	{
		mCurrentPlan[i] = -1;
	}

	if(mCurrentMap.isFree(mGoalPoint))//不是未知点也小于障碍点阈值
	{
		queue.insert(Entry(0.0, mGoalPoint));//distance = 0.0, index = mGoalPoint
		mCurrentPlan[mGoalPoint] = 0;
	}else
	{
		// Initialize the queue with area around the goal point
		int reach = mCellRobotRadius + (1.0 / mCurrentMap.getResolution());
		std::vector<unsigned int> neighbors = mCurrentMap.getFreeNeighbors(mGoalPoint, reach);
		for(unsigned int i = 0; i < neighbors.size(); i++)
		{
			queue.insert(Entry(0.0, neighbors[i]));
			mCurrentPlan[neighbors[i]] = 0;//没有标记的是-1
		}
	}

		
	Queue::iterator next;
	double distance;
	unsigned int x, y, index;
	double linear = mCurrentMap.getResolution();//0.05m
	double diagonal = std::sqrt(2.0) * linear;//0.05*sqrt(2)

	
	//以目标点为出发点，全图搜索机器人当前位置点
	// Do full search with Dijkstra-Algorithm
	while(!queue.empty())
	{
		// Get the nearest cell from the queue
		next = queue.begin();
		distance = next->first;
		index = next->second;
		queue.erase(next);
		
		if(mCurrentPlan[index] >= 0 && mCurrentPlan[index] < distance) continue;
		
		//if(index == mStartPoint) break;//是不是可以取消注释//不退出,整幅图像都会被搜索,这样的路径只能是最优路径
		
		// Add all adjacent cells
		if(!mCurrentMap.getCoordinates(x, y, index)) continue;
		std::vector<unsigned int> ind;
		ind.push_back(index - 1);
		ind.push_back(index + 1);
		ind.push_back(index - mCurrentMap.getWidth());
		ind.push_back(index + mCurrentMap.getWidth());
		ind.push_back(index - mCurrentMap.getWidth() - 1);
		ind.push_back(index - mCurrentMap.getWidth() + 1);
		ind.push_back(index + mCurrentMap.getWidth() - 1);
		ind.push_back(index + mCurrentMap.getWidth() + 1);
			
		for(unsigned int it = 0; it < ind.size(); ++it)
		{
			unsigned int i = ind[it];
			if(mCurrentMap.isFree(i))
			{
				double delta = (it < 4) ? linear : diagonal;
				//newDistance = distance + cost
				double newDistance = distance + delta + (10 * delta * (double)mCurrentMap.getData(i) / (double)mCostObstacle);
				if(mCurrentPlan[i] == -1 || newDistance < mCurrentPlan[i])
				{
					queue.insert(Entry(newDistance, i));
					mCurrentPlan[i] = newDistance;
				}
			}
		}
	}

	//mCurrentPlan是整幅图像，被初始化为-1
	if(mCurrentPlan[mStartPoint] < 0)
	{
		ROS_ERROR("No way between robot and goal!");
		return false;
	}
	
	publishPlan();
	return true;
}


void RobotNavigator::publishPlan()
{
	nav_msgs::Path pathData;
	pathData.header.frame_id = mMapFrame.c_str();
	pathData.header.stamp = Time::now();

	// nav_msgs::GridCells plan_msg;
	// plan_msg.header.frame_id = mMapFrame.c_str();
	// plan_msg.header.stamp = Time::now();
	
	// plan_msg.cell_width = mCurrentMap.getResolution();
	// plan_msg.cell_height = mCurrentMap.getResolution();
	
	unsigned int index = mStartPoint;
	std::vector<std::pair<double, double> > points;

	while(true)
	{
		unsigned int x = 0, y = 0;
		if(mCurrentMap.getCoordinates(x,y,index))
			points.push_back(std::pair<double, double>(
				((x+0.5) * mCurrentMap.getResolution()) + mCurrentMap.getOriginX(), 
				((y+0.5) * mCurrentMap.getResolution()) + mCurrentMap.getOriginY()
			));
		
		//代价为0，意味着找到目标点，则退出
		if(mCurrentPlan[index] == 0) break;
		
		unsigned int next_index = index;
		
		//寻找当前点周围代价最小的点
		std::vector<unsigned int> neighbors = mCurrentMap.getFreeNeighbors(index);
		for(unsigned int i = 0; i < neighbors.size(); ++i)
		{
			if(mCurrentPlan[neighbors[i]] >= 0 && mCurrentPlan[neighbors[i]] < mCurrentPlan[next_index])
				next_index = neighbors[i];
		}

		//如果当前点周围最小代价的点就是当前点，则失败退出
		if(index == next_index) break;
		index = next_index;
	}
	
	// plan_msg.cells.resize(points.size());
	// for(unsigned int i = 0; i < points.size(); i++)
	// {
	// 	plan_msg.cells[i].x = points[i].first;
	// 	plan_msg.cells[i].y = points[i].second;
	// 	plan_msg.cells[i].z = 0.0;
	// }
	// mPlanPublisher.publish(plan_msg);
	pathData.poses.resize(points.size());

	for(unsigned int i = 0; i < points.size(); ++i)
	{
		pathData.poses[i].pose.position.x = points[i].first;
		pathData.poses[i].pose.position.y = points[i].second;
		pathData.poses[i].pose.position.z = 0.f;
	}
	mPlanPublisher.publish(pathData);
}


bool RobotNavigator::getMap()
{
	//没有出现新地图就false
    if(!mapMsgflg) return false;

    mCurrentMap.update(mapMsg);
    
	//每次更新地图都要把这个更新??
    if(mCurrentPlan) delete[] mCurrentPlan;
    mCurrentPlan = new double[mCurrentMap.getSize()];

    if(mCellInflationRadius == 0)
    {
        //ROS_INFO("Navigator is now initialized.");
		mCellInflationRadius = mInflationRadius / mCurrentMap.getResolution();
		mCellRobotRadius = mRobotRadius / mCurrentMap.getResolution();
		mInflationTool.computeCaches(mCellInflationRadius);
		mCurrentMap.setLethalCost(mCostLethal);
    }

	ROS_ERROR("\n");
	ROS_ERROR("get new map"); 
	ROS_ERROR("width:%d, height:%d", mCurrentMap.getWidth(), mCurrentMap.getHeight());
	ROS_ERROR("origin x:%f, y:%f", mCurrentMap.getOriginX() / mCurrentMap.getResolution(),
									mCurrentMap.getOriginY() / mCurrentMap.getResolution());
    mHasNewMap = true;
	mapInflatedFlg = false;
    mapMsgflg = false;

    return true;
}

void mapSubscribeCallback(const nav_msgs::OccupancyGrid& grid)
{
    mapMsg = grid;
    mapMsgflg = true;
	ROS_ERROR("map call back");
}

bool RobotNavigator::setCurrentPosition()
{
	StampedTransform transform;
	try
	{
		mTfListener.lookupTransform(mMapFrame, mRobotFrame, Time(0), transform);
	}
	catch(TransformException ex)
	{
		ROS_ERROR("Could not get robot position: %s", ex.what());
		return false;
	}
	double world_x = transform.getOrigin().x();
	double world_y = transform.getOrigin().y();
	double world_theta = getYaw(transform.getRotation());
	
	unsigned int current_x = (world_x - mCurrentMap.getOriginX()) / mCurrentMap.getResolution();
	unsigned int current_y = (world_y - mCurrentMap.getOriginY()) / mCurrentMap.getResolution();
	unsigned int i;

	ROS_ERROR("transform x: %f, y:%f, current x:%d, y:%d", world_x, world_y, current_x, current_y);

	if(!mCurrentMap.getIndex(current_x, current_y, i))
	{
		if(mHasNewMap || !getMap() || !mCurrentMap.getIndex(current_x, current_y, i))
		{
			ROS_ERROR("Is the robot out of the map?");
			return false;
		}
	}
	mStartPoint = i;
	mCurrentDirection = world_theta;
	mCurrentPositionX = world_x;
	mCurrentPositionY = world_y;
	return true;
}