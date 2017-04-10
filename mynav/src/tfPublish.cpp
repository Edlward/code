#include "ros/ros.h"
#include "geometry_msgs/TransformStamped.h"
#include <tf2_msgs/TFMessage.h>
#include <tf/transform_broadcaster.h>
#include <vector>

void tfCallback(const tf2_msgs::TFMessage &msg);

std::vector<geometry_msgs::TransformStamped > transformStamped;
ros::Time current_time;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "tfPublish");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("tf", 100, tfCallback);
    tf::TransformBroadcaster tf_broadcaster;
    
    geometry_msgs::TransformStamped odom;
    tf::Transform transform;

    // float xTmp = 0.f;
    // float yTmp = 0.f;
    ros::Rate loop_rate(20.0);

    ROS_INFO("come to while");
    while(ros::ok())
    {
        // ROS_INFO("spinonce");
        
        if(transformStamped.size() > 1)
        {
            //ROS_INFO("size > 1");
            odom = transformStamped[1];

            tf::Vector3 v(odom.transform.translation.x,
                            odom.transform.translation.y,
                            0.0);
            tf::Quaternion q(odom.transform.rotation.x,
                                odom.transform.rotation.y,
                                odom.transform.rotation.z,
                                odom.transform.rotation.w);
            transform.setOrigin(v);
            transform.setRotation(q);

            tf::StampedTransform tfMapRobot(tf::StampedTransform(transform,
                                            ros::Time::now(),
                                            "map",
                                            "robot"));
            tf_broadcaster.sendTransform(tfMapRobot);
            
            ROS_INFO("transform.frame_id:%s", tfMapRobot.frame_id_.c_str());
            ROS_INFO("transform.child_frame_id:%s", tfMapRobot.child_frame_id_.c_str());

            float x = transform.getOrigin().x();
            float y = transform.getOrigin().y();
            // ROS_ERROR("transform x: %f, y:%f", x, y);

            transformStamped.clear();
        }
        ros::spinOnce();
        loop_rate.sleep();        
    }
    return 1;
}


void tfCallback(const tf2_msgs::TFMessage &msg)
{
    //   printf("get callback\n");   
    transformStamped = msg.transforms;

}