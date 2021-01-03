#include <iostream>
#include "MazePoint.h"
#include "Maze.h"
#include "MazePathFinder.h"
#include "MazePathFindingManager.h"

int main()
{
	Maze maze;
	if (!maze.processInputMazeFile("Maze.txt"))
	{
		return 1;
	}

	std::mutex printMutex;

	MazePathFindingManager findingManager(maze, printMutex, 5);

	auto completedPath = findingManager.findPath(MazePoint(19, 15));
	if (!completedPath.empty())
	{
		for (auto const& point : completedPath)
		{
			std::cout << point.getRowIndex() << " " 
				<< point.getColumnIndex() << std::endl;
		}
		maze.printMazeWithPath(completedPath);
	}
	else
	{
		std::cout << "No complete path found" << std::endl;
	}

	return 0;
}
