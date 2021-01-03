#include <iostream>
#include <algorithm>
#include "MazePathFinder.h"
#include "MazePathFinderParams.h"

MazePathFinder::MazePathFinder(MazePathFinderParams& finderParams_)
	: finderParams(finderParams_)
{
}

void MazePathFinder::findPath(MazePoint const& startingPoint)
{
	partialPaths.push({ startingPoint });

	while (!partialPaths.empty())
	{
		auto currentPath = partialPaths.front();
		partialPaths.pop();

		auto const endPoint = currentPath.back();

		std::vector<MazePath> newPaths;

		createNewPaths(endPoint, currentPath, newPaths);
		if (!completePath.empty())
		{
			break;
		}

		for (auto const& path : newPaths)
		{
			partialPaths.push(path);
		}
	}

	std::cout << "Path finding completed" << std::endl;
}

MazePath const& MazePathFinder::getCompletePath() const
{
	return completePath;
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
		!finderParams.maze.isBrick({upperRowIndex, endPoint.getColumnIndex()}))
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
	if (lowerRowIndex < static_cast<int>(finderParams.maze.getNumRows()) &&
		!finderParams.maze.isBrick({ lowerRowIndex, endPoint.getColumnIndex() }))
	{
		auto newPathPoint = MazePoint(lowerRowIndex,
			endPoint.getColumnIndex());

		createNewPath(currentPath, newPathPoint, newPaths,
			lowerRowIndex, finderParams.maze.getNumRows() - 1);
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
		!finderParams.maze.isBrick({ endPoint.getRowIndex(), leftColumnIndex }))
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
	auto rightColumnIndex = endPoint.getColumnIndex() - 1;
	if (rightColumnIndex < static_cast<int>(finderParams.maze.getNumColumns()) &&
		!finderParams.maze.isBrick({ endPoint.getRowIndex(), rightColumnIndex }))
	{
		auto newPathPoint = MazePoint(endPoint.getRowIndex(),
			rightColumnIndex);

		createNewPath(currentPath, newPathPoint, newPaths,
			rightColumnIndex, finderParams.maze.getNumColumns() - 1);
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
			std::cout << "Found complete path" << std::endl;
			completePath = newPath;
		}
	}
}

