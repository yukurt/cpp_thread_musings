#include "MazePath.h"

MazePath::MazePath()
{
}

MazePath::MazePath(std::initializer_list<MazePoint> init)
	: std::vector<MazePoint>(init)
{
}
