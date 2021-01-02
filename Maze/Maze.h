#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "MazePoint.h"
#include "MazePath.h"

class Maze
{
public:
	const char BRICK_SYMBOL = '#';
	const char PATH_SYMBOL  = '.';

	bool processInputMazeFile(std::string const& inputMazeFile);
	void printMaze() const;
	std::size_t getNumRows() const;
	std::size_t getNumColumns() const;
	void printMazeWithPath(MazePath const& path) const;
	bool isBrick(MazePoint const& point) const;

private:
	std::vector<std::string> matrix;
	std::size_t numRows = 0;
	std::size_t numColumns = 0;
};

