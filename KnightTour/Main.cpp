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
	const std::size_t numFullToursRequired = 2;
	const auto numThreads = 10;

	KnightTouristsManager<BOARD_LENGTH> touristsManager;
	touristsManager.findFullTours(numFullToursRequired, numThreads);
	printFullTours(touristsManager);

	return 0;
}

template <std::size_t BOARD_LENGTH>
void printFullTours(KnightTouristsManager<BOARD_LENGTH> const& 
	touristsManager)
{
	for (auto const& fullTour : touristsManager.getFullTours())
	{
		fullTour.dump();
		std::cout << std::endl;
	}
}
