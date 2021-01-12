#pragma once

#include <vector>
#include <latch>
#include <mutex>

#include "ChessBoard.h"
#include "KnightTourist.h"

template <std::size_t BOARD_LENGTH>
class KnightTouristsManager
{
public:
	KnightTouristsManager();

	void findClosedTours(
		const std::size_t numClosedToursRequired, 
		const std::size_t numThreads);

	std::vector<ChessBoard<BOARD_LENGTH>> const&
		getClosedTours() const;

private:
	void reset();
	void createTourists(const size_t& numThreads, 
		std::latch& closedTourLatch);
	void createTouristThreads();
	void joinTouristThreads();

	std::vector<KnightTourist<BOARD_LENGTH>> tourists;
	std::vector<std::thread> touristThreads;

	std::mutex closedTourMutex;
	bool stopFinding = false;

	std::vector<ChessBoard<BOARD_LENGTH>> closedTourBoards;
};

template<std::size_t BOARD_LENGTH>
inline KnightTouristsManager<BOARD_LENGTH>::KnightTouristsManager()
{
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::findClosedTours(
		const std::size_t numClosedToursRequired,
		const std::size_t numThreads)
{
	reset();

	std::latch closedTourLatch{ 
		static_cast<std::ptrdiff_t>(numClosedToursRequired) };

	createTourists(numThreads, closedTourLatch);
	createTouristThreads();

	closedTourLatch.wait();
	stopFinding = true;

	joinTouristThreads();
}

template<std::size_t BOARD_LENGTH>
inline std::vector<ChessBoard<BOARD_LENGTH>> const& 
	KnightTouristsManager<BOARD_LENGTH>::getClosedTours() const
{
	return closedTourBoards;
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::joinTouristThreads()
{
	for (auto& touristThread : touristThreads)
	{
		touristThread.join();
	}
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::createTouristThreads()
{
	for (auto& tourist : tourists)
	{
		touristThreads.emplace_back(
			&KnightTourist<BOARD_LENGTH>::findClosedTours, &tourist);
	}
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::reset()
{
	tourists.clear();
	touristThreads.clear();
	stopFinding = false;
	closedTourBoards.clear();
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::createTourists(
	const size_t& numThreads, std::latch& closedTourLatch)
{
	tourists.clear();
	for (std::size_t i = 0; i < numThreads; ++i)
	{
		tourists.emplace_back(closedTourLatch, closedTourBoards,
			closedTourMutex, stopFinding);
	}
}
