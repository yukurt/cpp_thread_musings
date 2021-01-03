#include <iostream>
#include <algorithm>
#include "MazePathFinder.h"

MazePathFinder::MazePathFinder
(
	  std::size_t id_
	, const Maze& maze_
	, std::mutex& printMutex_
	, std::queue<MazePath>& partialPaths_
	, std::mutex& partialPathsMutex_
	, bool& stopFinding_
)
	: id(id_)
	, maze(maze_)
	, printMutex(printMutex_)
	, partialPaths(partialPaths_)
	, partialPathsMutex(partialPathsMutex_)
	, stopFinding(stopFinding_)
{}

MazePath MazePathFinder::findPath()
{
	while (true)
	{
		if (stopFinding)
		{
			std::lock_guard<std::mutex> plock(printMutex);
			std::cout << id << ": force stoppage" << std::endl;
			break;
		}

		MazePath currentPath;

		{
			std::unique_lock<std::mutex> lock(partialPathsMutex);
			if (partialPaths.empty())
			{
				lock.unlock();
				std::this_thread::yield();
				continue;
			}

			isBusySearching = true;

			currentPath = partialPaths.front();
			partialPaths.pop();

			/*std::lock_guard<std::mutex> plock();
			std::cout << id << ": ";
			for (auto const& point : currentPath)
			{
				std::cout << "(" << point.getRowIndex() << ","
					<< point.getColumnIndex() << ") ";
			}
			std::cout << std::endl;*/
		}

		auto const endPoint = currentPath.back();

		std::vector<MazePath> newPaths;

		createNewPaths(endPoint, currentPath, newPaths);
		if (!completePath.empty())
		{
			std::lock_guard<std::mutex> lock(printMutex);
			std::cout << id << ": found complete path" << std::endl;
			std::cout << id << ": ";
			for (auto const& point : completePath)
			{
				std::cout << "(" << point.getRowIndex() << ","
					<< point.getColumnIndex() << ") ";
			}
			std::cout << std::endl;
			isBusySearching = false;
			break;
		}

		if (!newPaths.empty())
		{
			std::lock_guard<std::mutex> lock(partialPathsMutex);
			for (auto const& path : newPaths)
			{
				partialPaths.push(path);
			}
		}

		isBusySearching = false;
	}

	return completePath;
}

MazePath const& MazePathFinder::getCompletePath() const
{
	return completePath;
}

bool MazePathFinder::isBusyFinding() const
{
	return isBusySearching;
}

void MazePathFinder::createNewPaths
(
	const MazePoint& endPoint
	, MazePath& currentPath
	, std::vector<MazePath>& newPaths
)
{
	createNewUpPath(endPoint, currentPath, newPaths);
	if (!completePath.empty())
	{
		return;
	}

	createNewDownPath(endPoint, currentPath, newPaths);
	if (!completePath.empty())
	{
		return;
	}

	createNewLeftPath(endPoint, currentPath, newPaths);
	if (!completePath.empty())
	{
		return;
	}

	createNewRightPath(endPoint, currentPath, newPaths);
	if (!completePath.empty())
	{
		return;
	}
}

void MazePathFinder::createNewUpPath
(
	  const MazePoint& endPoint
	, const MazePath& currentPath
	, std::vector<MazePath>& newPaths
)
{
	auto upperRowIndex = endPoint.getRowIndex() - 1;
	if (upperRowIndex >= 0 && 
		!maze.isBrick({upperRowIndex, endPoint.getColumnIndex()}))
	{
		auto newPathPoint = MazePoint(upperRowIndex,
			endPoint.getColumnIndex());

		createNewPath(currentPath, newPathPoint, newPaths, upperRowIndex, 0);
	}
}

void MazePathFinder::createNewDownPath
(
	  const MazePoint& endPoint
	, const MazePath& currentPath
	, std::vector<MazePath>& newPaths
)
{
	auto lowerRowIndex = endPoint.getRowIndex() + 1;
	if (lowerRowIndex < static_cast<int>(maze.getNumRows()) &&
		!maze.isBrick({ lowerRowIndex, endPoint.getColumnIndex() }))
	{
		auto newPathPoint = MazePoint(lowerRowIndex,
			endPoint.getColumnIndex());

		createNewPath(currentPath, newPathPoint, newPaths,
			lowerRowIndex, maze.getNumRows() - 1);
	}
}

void MazePathFinder::createNewLeftPath
(
	  const MazePoint& endPoint
	, const MazePath& currentPath
	, std::vector<MazePath>& newPaths
)
{
	auto leftColumnIndex = endPoint.getColumnIndex() - 1;
	if (leftColumnIndex >= 0 &&
		!maze.isBrick({ endPoint.getRowIndex(), leftColumnIndex }))
	{
		auto newPathPoint = MazePoint(endPoint.getRowIndex(), leftColumnIndex);

		createNewPath(currentPath, newPathPoint, newPaths, leftColumnIndex, 0);
	}
}

void MazePathFinder::createNewRightPath
(
	  const MazePoint& endPoint
	, const MazePath& currentPath
	, std::vector<MazePath>& newPaths
)
{
	auto rightColumnIndex = endPoint.getColumnIndex() + 1;
	if (rightColumnIndex < static_cast<int>(maze.getNumColumns()) &&
		!maze.isBrick({ endPoint.getRowIndex(), rightColumnIndex }))
	{
		auto newPathPoint = MazePoint(endPoint.getRowIndex(),
			rightColumnIndex);

		createNewPath(currentPath, newPathPoint, newPaths,
			rightColumnIndex, maze.getNumColumns() - 1);
	}
}

void MazePathFinder::createNewPath
(
	  const MazePath& currentPath
	, MazePoint& newPathPoint
	, std::vector<MazePath>& newPaths
	, MazePoint::Index newIndex
	, MazePoint::Index indexBorder
)
{
	auto samePoint = std::find(currentPath.begin(), currentPath.end(), newPathPoint);
	if (samePoint == currentPath.end())
	{
		auto newPath = currentPath;
		newPath.push_back(newPathPoint);
		newPaths.push_back(newPath);

		if (newIndex == indexBorder)
		{
			//std::cout << "Found complete path" << std::endl;
			completePath = newPath;
		}
	}
}

