#include "MazePathFinderParams.h"

MazePathFinderParams::MazePathFinderParams
(
	  std::size_t id_
	, std::mutex& printMutex_
	, Maze& maze_
)
	: id(id_)
	, printMutex(printMutex_)
	, maze(maze_)
{}
