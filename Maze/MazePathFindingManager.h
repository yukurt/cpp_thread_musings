#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>

#include "MazePath.h"
#include "MazePathFinder.h"

class Maze;

class MazePathFindingManager
{
public:
	MazePathFindingManager(const Maze& maze_, std::mutex& printMutex_,
		std::size_t numThreads);
	MazePath findPath(MazePoint const& startingPoint);

private:
	void inspectThreads(MazePath& completePath);
	void pushStartingPoint(const MazePoint& startingPoint);
	void launchThreads();

private:
	Maze const& maze;
	std::mutex& printMutex;
	std::size_t numThreads;

	std::vector<MazePathFinder> pathFinders;

	std::queue<MazePath> partialPaths;
	std::mutex partialPathsMutex;

	std::chrono::duration<std::size_t, std::milli> threadWaitTime{ 100 };

	bool stopFinding = false;

	std::vector<std::future<MazePath>> completePathsFutures;
};

