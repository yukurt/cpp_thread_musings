#include <iostream>
#include <mutex>
#include <vector>

#include "MazePoint.h"
#include "Maze.h"
#include "MazePathFinder.h"
#include "MazePathFinderParams.h"

int main()
{
	std::mutex printMutex;

	Maze maze;
	if (!maze.processInputMazeFile("Maze.txt"))
	{
		return 1;
	}

	const std::size_t numThreads = 1;

	std::vector<MazePathFinderParams> finderParamsCollections;
	for (std::size_t i = 0; i < numThreads; ++i)
	{
		finderParamsCollections.emplace_back(i, printMutex, maze);
	}

	MazePathFinder pathFinder(finderParamsCollections[0]);
	pathFinder.findPath(MazePoint(19, 15));

	if (!pathFinder.getCompletePath().empty())
	{
		for (auto const& point : pathFinder.getCompletePath())
		{
			std::cout << point.getRowIndex() << " " 
				<< point.getColumnIndex() << std::endl;
		}
		maze.printMazeWithPath(pathFinder.getCompletePath());
	}
	else
	{
		std::cout << "No complete path found" << std::endl;
	}

	return 0;
}
