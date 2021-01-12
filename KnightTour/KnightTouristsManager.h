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

	void findFullTours(
		const std::size_t numFullToursRequired, 
		const std::size_t numThreads);

	std::vector<ChessBoard<BOARD_LENGTH>> const&
		getFullTours() const;

private:
	void reset();
	void createTourists(const size_t& numThreads, 
		std::latch& fullTourLatch);
	void createTouristThreads();
	void joinTouristThreads();

	std::vector<KnightTourist<BOARD_LENGTH>> tourists;
	std::vector<std::thread> touristThreads;

	std::mutex fullTourMutex;
	bool stopFinding = false;

	std::vector<ChessBoard<BOARD_LENGTH>> fullTourBoards;
};

template<std::size_t BOARD_LENGTH>
inline KnightTouristsManager<BOARD_LENGTH>::KnightTouristsManager()
{
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::findFullTours(
		const std::size_t numFullToursRequired,
		const std::size_t numThreads)
{
	reset();

	std::latch fullTourLatch{ 
		static_cast<std::ptrdiff_t>(numFullToursRequired) };

	createTourists(numThreads, fullTourLatch);
	createTouristThreads();

	fullTourLatch.wait();
	stopFinding = true;

	joinTouristThreads();
}

template<std::size_t BOARD_LENGTH>
inline std::vector<ChessBoard<BOARD_LENGTH>> const& 
	KnightTouristsManager<BOARD_LENGTH>::getFullTours() const
{
	return fullTourBoards;
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
			&KnightTourist<BOARD_LENGTH>::findFullTours, &tourist);
	}
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::reset()
{
	tourists.clear();
	touristThreads.clear();
	stopFinding = false;
	fullTourBoards.clear();
}

template<std::size_t BOARD_LENGTH>
void KnightTouristsManager<BOARD_LENGTH>::createTourists(
	const size_t& numThreads, std::latch& fullTourLatch)
{
	tourists.clear();
	for (std::size_t i = 0; i < numThreads; ++i)
	{
		tourists.emplace_back(fullTourLatch, fullTourBoards,
			fullTourMutex, stopFinding);
	}
}
