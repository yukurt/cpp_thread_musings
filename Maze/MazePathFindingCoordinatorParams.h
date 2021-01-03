#pragma once

#include <vector>
#include <mutex>

#include "MazePathFinderParams.h"

class Maze;

struct MazePathFindingCoordinatorParams
{
public:
	MazePathFindingCoordinatorParams
	(
		  std::mutex& printMutex_
		, std::vector<MazePathFinderParams>& finderParamsCollection_
	);

	std::mutex& printMutex;
	std::vector<MazePathFinderParams>& finderParamsCollection;
};

