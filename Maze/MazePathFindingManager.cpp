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
	MazePath completedPath;

	std::vector<std::future<MazePath>> completedPathFutures{ numThreads };
	for (auto i = 0u; i < numThreads; ++i)
	{
		completedPathFutures[i] = std::async(std::launch::async,
			&MazePathFinder::findPath, &pathFinders[i]);
	}

	{
		std::lock_guard<std::mutex> lock(partialPathsMutex);
		partialPaths.push({ startingPoint });
	}

	while (true)
	{
		for (auto i = 0u; i < numThreads; ++i)
		{
			auto status = completedPathFutures[i].wait_for(threadWaitTime);
			if (status == std::future_status::ready)
			{
				completedPath = completedPathFutures[i].get();
				stopFinding = true;
				break;
			}
		}

		if (stopFinding)
		{
			break;
		}

		{
			std::lock_guard<std::mutex> lock(partialPathsMutex);
			if (partialPaths.empty())
			{
				stopFinding = true;
				break;
			}
		}
	}

	return completedPath;
}
