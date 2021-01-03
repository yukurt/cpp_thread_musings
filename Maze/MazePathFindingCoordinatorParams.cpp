#include "MazePathFindingCoordinatorParams.h"

MazePathFindingCoordinatorParams::MazePathFindingCoordinatorParams
(
  	  std::mutex& printMutex_
	, std::vector<MazePathFinderParams>& finderParamsCollection_
)
	: printMutex(printMutex_)
	, finderParamsCollection(finderParamsCollection_)
{}
