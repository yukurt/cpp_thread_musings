#include <iostream>

#include "MazePoint.h"
#include "Maze.h"
#include "MazePathFinder.h"
#include "MazePathFindingManager.h"
#include "main.h"

int main()
{
	std::string inputMazeFile = "Maze.txt";
	const int numThreads = 10;
	auto const startingPoint = MazePoint(15, 0);

	Maze maze;
	if (!maze.processInputMazeFile(inputMazeFile))
	{
		return 1;
	}

	std::mutex printMutex;
	MazePathFindingManager findingManager(maze, printMutex, numThreads);

	auto completePath = findingManager.findPath(startingPoint);
	printResult(printMutex, completePath, maze);

	return 0;
}

void printResult(std::mutex& printMutex, MazePath& completePath, Maze& maze)
{
	std::lock_guard<std::mutex> lock(printMutex);
	if (!completePath.empty())
	{
		maze.printMazeWithPath(completePath);
	}
	else
	{
		std::cout << "No complete path found" << std::endl;
	}
}
