#include "navigation.h"

/*
rosrun mynav navigator
roslaunch mynav tutorial3.launch
*/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "navigator");
    ros::NodeHandle n;
    RobotNavigator robnav;
    
    robnav.exploreGoGoal();

    ros::spin();
    return 0;
}