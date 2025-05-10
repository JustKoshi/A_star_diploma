#pragma once
#include "Node.h"
#include <iostream>
struct stats_str {
	heuristicType heuristic;
	int pathLength = 0;
	int iterations;
	double timeElapsed;
	double ratio_iterations_pathLength;
	double ratio_visted_all;
	stats_str(heuristicType type, int pathlen, int iter, double time, double ratio1, double ratio2): 
	heuristic(type), pathLength(pathlen), iterations(iter), timeElapsed(time), ratio_iterations_pathLength(ratio1), ratio_visted_all(ratio2) {}
	stats_str(): heuristic(heuristicType::manhattan), pathLength(0), iterations(0), timeElapsed(0.0), ratio_iterations_pathLength(0.0), ratio_visted_all(0.0) {}
};

