#pragma once

#include <mutex>
#include <future>

#include "MazePath.h"

class Maze;
class MazePath;

struct MazePathFinderParams
{
public:
	MazePathFinderParams
	(
		std::size_t id_
		, std::mutex& printMutex_
		, Maze& maze_
	);

	std::size_t id;
	std::mutex& printMutex;
	Maze& maze;

	bool isBusy = false;
	std::future<MazePath> completedPath;
};
