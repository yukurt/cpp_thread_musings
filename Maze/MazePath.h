#pragma once

#include <vector>
#include "MazePoint.h"

class MazePath: public std::vector<MazePoint>
{
public:
	MazePath();
	MazePath(std::initializer_list<MazePoint> init);
};

