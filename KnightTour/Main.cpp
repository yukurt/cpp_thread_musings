#include <iostream>
#include <latch>
#include <mutex>
#include <vector>
#include <thread>

#include "ChessBoard.h"
#include "KnightTourist.h"

int main()
{
	const std::size_t BOARD_LENGTH = 8;
	const std::size_t numClosedToursRequired = 2;
	const auto numThreads = 10;

	std::latch closedTourLatch{numClosedToursRequired};
	std::vector<ChessBoard<BOARD_LENGTH>> closedTourBoards;
	std::mutex closedTourMutex;
	bool stopFinding = false;

	std::vector<KnightTourist<BOARD_LENGTH>> tourists;

	std::vector<std::thread> touristThreads;
	for (int i = 0; i < numThreads; ++i)
	{
		tourists.emplace_back(closedTourLatch, closedTourBoards,
			closedTourMutex, stopFinding);
	}

	for (auto& tourist : tourists)
	{
		touristThreads.emplace_back(
			&KnightTourist<BOARD_LENGTH>::findClosedTours, &tourist);
	}

	closedTourLatch.wait();
	stopFinding = true;

	for (auto const& closedTour : closedTourBoards)
	{
		closedTour.dump();
		std::cout << std::endl;
	}

	for (auto& touristThread : touristThreads)
	{
		touristThread.join();
	}

	return 0;
}