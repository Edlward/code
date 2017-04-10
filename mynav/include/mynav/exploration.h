#ifndef _EXPLORATION_H_
#define _EXPLORATION_H_

#include "gridmap.h"

#define EXPL_TARGET_SET 1
#define EXPL_FINISHED   2
#define EXPL_WAITING    3
#define EXPL_FAILED     4

// The base class for all exploration planners
class ExplorationPlanner
{
public:
	virtual ~ExplorationPlanner() {};
	virtual int findExplorationTarget(GridMap* map, unsigned int start, unsigned int &goal) = 0;
	
};

#endif