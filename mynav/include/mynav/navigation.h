#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <nav_msgs/OccupancyGrid.h>

#include "gridmap.h"
#include "mapinflation.h"
#include "exploration.h"

#include <queue>



class RobotNavigator
{
public:
	RobotNavigator();
	~RobotNavigator();

	void exploreGoGoal();
	
private:
	bool isLocalized();
	bool setCurrentPosition();
	bool getMap();
	void stop();
	bool preparePlan();
	bool createPlan();
	void publishPlan();
    
    // Everything related to ROS
	tf::TransformListener mTfListener;
	ros::Subscriber mMapSubscriber;
	ros::Publisher mMapPublisher;
	ros::Publisher mPlanPublisher;
	ros::Publisher mMarkerPublisher;

    std::string mMapFrame;
	std::string mRobotFrame;

	bool mapInflatedFlg;
    // Current status and goals
	bool mHasNewMap;
	bool mIsPaused;
	bool mIsStopped;
	int mStatus;// move status
	int mRobotID;
	unsigned int mGoalPoint;
	unsigned int mStartPoint;
	double mCurrentDirection;
	double mCurrentPositionX;
	double mCurrentPositionY;
    	// Everything related to the global map and plan
	MapInflationTool mInflationTool;
	std::string mExplorationStrategy;
	boost::shared_ptr<ExplorationPlanner> mExplorationPlanner;
	GridMap mCurrentMap;
	double* mCurrentPlan;

	double mInflationRadius;
	double mRobotRadius;
	unsigned int mCellInflationRadius;
	unsigned int mCellRobotRadius;

	char mCostObstacle;
	char mCostLethal;

	double mNavigationGoalDistance;
	double mNavigationGoalAngle;
	double mNavigationHomingDistance;
	double mExplorationGoalDistance;
	double mMinReplanningPeriod;
	double mMaxReplanningPeriod;
};

#endif