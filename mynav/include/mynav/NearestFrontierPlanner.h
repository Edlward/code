#ifndef _NEARESTFRONTIERPLANNER_H_
#define _NEARESTFRONTIERPLANNER_H_

#include <exploration.h>

class NearestFrontierPlanner : public ExplorationPlanner
{
	public:
		NearestFrontierPlanner();
		~NearestFrontierPlanner();

		int findExplorationTarget(GridMap* map, unsigned int start, unsigned int &goal);

	private:

};

#endif // NEARESTFRONTIERPLANNER_H_
