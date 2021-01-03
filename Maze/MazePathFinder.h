#pragma once

#include <vector>
#include <queue>

#include "MazePoint.h"
#include "MazePath.h"
#include "Maze.h"

class Maze;
struct MazePathFinderParams;

class MazePathFinder
{
public:
	MazePathFinder(MazePathFinderParams& finderParams_);

	void findPath(MazePoint const& startingPoint);
	MazePath const& getCompletePath() const;

private:
	void createNewPaths
	(
		  const MazePoint& endPoint
		, MazePath& currentPath
		, std::vector<MazePath>& newPaths
	);
	void createNewUpPath
	(
		  const MazePoint& endPoint
		, const MazePath& currentPath
		, std::vector<MazePath>& newPaths
	);
	void createNewDownPath
	(
		  const MazePoint& endPoint
		, const MazePath& currentPath
		, std::vector<MazePath>& newPaths
	);	
	void createNewLeftPath
	(
  		  const MazePoint& endPoint
		, const MazePath& currentPath
		, std::vector<MazePath>& newPaths
	);
	void createNewRightPath
	(
		  const MazePoint& endPoint
		, const MazePath& currentPath
		, std::vector<MazePath>& newPaths
	);
	void createNewPath
	(
		  const MazePath& currentPath
		, MazePoint& newPathPoint
		, std::vector<MazePath>& newPaths
		, MazePoint::Index newIndex
		, MazePoint::Index indexBorder
	);

private:
	MazePath completePath;
	std::queue<MazePath> partialPaths;
	MazePathFinderParams& finderParams;
};

