#ifndef _COMMAND_H_
#define _COMMAND_H_


#define NAV_ST_IDLE	      0  //闲置状态
#define NAV_ST_NAVIGATING 1  
#define NAV_ST_EXPLORING  4  //搜索目标位置 receiveExploreGoal
#define NAV_ST_WAITING    5  //速度、角速度设置为0，停止运动
#define NAV_ST_RECOVERING 6  //？

#endif