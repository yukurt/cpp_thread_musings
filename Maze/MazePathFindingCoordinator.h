#pragma once

#include <vector>
#include <thread>

#include "MazePathFinder.h"
#include "MazePathFindingCoordinatorParams.h"

class MazePath;

class MazePathFindingCoordinator
{
public:
	MazePathFindingCoordinator(
		MazePathFindingCoordinatorParams& coordinatorParams_);
	void findPath(MazePoint const& startingPoint);

private:
	MazePathFindingCoordinatorParams& coordinatorParams;
	std::vector<MazePathFinder> pathFinders;
	std::vector <std::thread> findingThreads;
};

