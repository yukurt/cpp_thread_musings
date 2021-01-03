#pragma once

#include <vector>
#include <queue>
#include <mutex>

#include "MazePoint.h"
#include "MazePath.h"
#include "Maze.h"

class Maze;

class MazePathFinder
{
public:
	MazePathFinder
	(
		  std::size_t id_
		, const Maze& maze_
		, std::mutex& printMutex_
		, std::queue<MazePath>& partialPaths_
		, std::mutex& partialPathsMutex_
		, bool& stopFinding_
	);
	MazePath findPath();
	void addNewPaths(std::vector<MazePath>& newPaths);
	void printCompletePath();
	MazePath getPartialPath();
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
	std::size_t id;
	Maze const& maze;
	std::mutex& printMutex;
	std::queue<MazePath>& partialPaths;
	std::mutex& partialPathsMutex;
	bool& stopFinding;
	MazePath completePath;
	bool debug = false;
};

