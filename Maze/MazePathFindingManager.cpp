#include <iostream>
#include <future>

#include "MazePathFindingManager.h"

MazePathFindingManager::MazePathFindingManager(const Maze& maze_,
	std::mutex& printMutex_, std::size_t numThreads_)
	: maze(maze_), printMutex(printMutex_), numThreads(numThreads_)
{
	for (auto i = 0u; i < numThreads; ++i)
	{
		pathFinders.emplace_back(i, maze, printMutex,
			partialPaths, partialPathsMutex, stopFinding);
	}
}

MazePath MazePathFindingManager::findPath(MazePoint const& startingPoint)
{
	MazePath completePath;

	launchThreads();
	pushStartingPoint(startingPoint);

	while (true)
	{
		inspectThreads(completePath);
		if (stopFinding)
		{
			break;
		}

		if (partialPaths.empty())
		{
			stopFinding = true;
			break;
		}
	}

	std::this_thread::sleep_for(threadWaitTime);
	return completePath;
}

void MazePathFindingManager::inspectThreads(MazePath& completePath)
{
	for (auto i = 0u; i < numThreads; ++i)
	{
		auto status = completePathsFutures[i].wait_for(threadWaitTime);
		if (status == std::future_status::ready)
		{
			completePath = completePathsFutures[i].get();
			stopFinding = true;
			break;
		}
	}
}

void MazePathFindingManager::pushStartingPoint(const MazePoint& startingPoint)
{
	{
		std::lock_guard<std::mutex> lock(partialPathsMutex);
		partialPaths.push({ startingPoint });
	}
}

void MazePathFindingManager::launchThreads()
{
	for (auto i = 0u; i < numThreads; ++i)
	{
		completePathsFutures.push_back(std::async(std::launch::async,
			&MazePathFinder::findPath, &pathFinders[i]));
	}
}
