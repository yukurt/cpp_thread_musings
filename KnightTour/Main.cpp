#include <iostream>
#include <latch>
#include <mutex>
#include <vector>
#include <thread>

#include "ChessBoard.h"
#include "KnightTouristsManager.h"
#include "Main.h"

int main()
{
	const std::size_t BOARD_LENGTH = 8;
	const std::size_t numClosedToursRequired = 2;
	const auto numThreads = 10;

	KnightTouristsManager<BOARD_LENGTH> touristsManager;
	touristsManager.findClosedTours(numClosedToursRequired, numThreads);
	printClosedTours(touristsManager);

	return 0;
}

template <std::size_t BOARD_LENGTH>
void printClosedTours(KnightTouristsManager<BOARD_LENGTH> const& 
	touristsManager)
{
	for (auto const& closedTour : touristsManager.getClosedTours())
	{
		closedTour.dump();
		std::cout << std::endl;
	}
}
