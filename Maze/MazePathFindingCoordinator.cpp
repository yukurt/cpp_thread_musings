#include "MazePathFindingCoordinator.h"

MazePathFindingCoordinator::MazePathFindingCoordinator(
	MazePathFindingCoordinatorParams& coordinatorParams_)
	: coordinatorParams(coordinatorParams_)
{
	for (auto& finderParam : coordinatorParams.finderParamsCollection)
	{
		pathFinders.emplace_back(finderParam);
	}
}

void MazePathFindingCoordinator::findPath(MazePoint const& startingPoint)
{
	for (auto& finder : pathFinders)
	{

	}
}
