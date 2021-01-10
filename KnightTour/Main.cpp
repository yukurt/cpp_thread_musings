#include <iostream>
#include <latch>
#include <mutex>
#include <vector>

#include "ChessBoard.h"
#include "KnightTourist.h"

int main()
{
	const std::size_t BOARD_LENGTH = 8;
	const std::size_t numClosedToursRequired = 1;

	std::latch closedTourLatch{numClosedToursRequired};
	std::vector<ChessBoard<BOARD_LENGTH>> closedTourBoards;
	std::mutex closedTourMutex;

	KnightTourist<BOARD_LENGTH> tourist(closedTourLatch,
		closedTourBoards, closedTourMutex);

	tourist.findClosedTour();
	tourist.showTour();

	return 0;
}