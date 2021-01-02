#include <iostream>
#include "MazePoint.h"
#include "Maze.h"
#include "MazePathFinder.h"

int main()
{
	Maze maze;
	if (!maze.processInputMazeFile("Maze.txt"))
	{
		return 1;
	}

	MazePathFinder pathFinder(maze);
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
