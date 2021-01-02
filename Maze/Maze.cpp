#include <iostream>
#include "Maze.h"

bool Maze::processInputMazeFile(std::string const& inputMazeFile)
{
	std::ifstream inFile(inputMazeFile);
	std::string line;

	matrix.clear();
	while (std::getline(inFile, line))
	{
		matrix.push_back(std::move(line));
	}

	numRows = matrix.size();
	if (!numRows)
	{
		std::cout << "No maze found in input file " 
			<< inputMazeFile << std::endl;
		return false;
	}

	numColumns = matrix[0].size();
	if (!numColumns)
	{
		std::cout << "Number of columns has to be at least 1" << std::endl;
		return false;
	}

	return true;
}

void Maze::printMaze() const
{
	for (auto const& row : matrix)
	{
		std::cout << row << std::endl;
	}
}

std::size_t Maze::getNumRows() const
{
	return numRows;
}

std::size_t Maze::getNumColumns() const
{
	return numColumns;
}

void Maze::printMazeWithPath(MazePath const& path) const
{
	auto matrixWithPath = matrix;

	for (auto const& point : path)
	{
		matrixWithPath[point.getRowIndex()][point.getColumnIndex()] = 
			PATH_SYMBOL;
	}

	for (auto const& row : matrixWithPath)
	{
		std::cout << row << "\n";
	}
}

bool Maze::isBrick(MazePoint const& point) const
{
	auto pointContent = matrix[point.getRowIndex()][point.getColumnIndex()];
	return pointContent == BRICK_SYMBOL;
}
