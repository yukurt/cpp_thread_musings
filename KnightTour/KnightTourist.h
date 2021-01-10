#pragma once

#include <random>
#include <algorithm>
#include <mutex>
#include <latch>
#include <vector>

#include "ChessBoard.h"

template <std::size_t BOARD_LENGTH>
class KnightTourist
{
public:
	enum class KnightMove
	{
		  upperLeftHorizontal
		, upperLeftVertical
		, upperRightVertical
		, upperRightHorizontal
		, lowerLeftHorizontal
		, lowerLeftVertical
		, lowerRightVertical
		, lowerRightHorizontal
		, totalKnightMoves
	};

	KnightTourist(std::latch& closedToursToFind_,
		std::vector<ChessBoard<BOARD_LENGTH>>& closedTourBoards_,
		std::mutex& closedTourMutex_);

	void findClosedTour();
	void showTour() const;

private:
	std::latch& closedToursToFind;
	std::vector<ChessBoard<BOARD_LENGTH>>& closedTourBoards;
	std::mutex& closedTourMutex;

	void initializeRandomEngine();
	BoardLocation getNewLocation(BoardLocation const& startingLocation,
		KnightMove knightMove) const;
	bool findNewValidLocation(BoardLocation const& startingLocation,
		BoardLocation& newValidLocation);

	ChessBoard<BOARD_LENGTH> board;

	std::mt19937 randomEngine;
	std::uniform_int_distribution<> randomBoardIndexGenerator
		{ 1, BOARD_LENGTH };
	std::uniform_int_distribution<> randomKnightMoveGenerator
		{ 0, static_cast<int>(KnightMove::totalKnightMoves) - 1 };
};

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::initializeRandomEngine()
{
	std::random_device rd;
	std::array<int, std::mt19937::state_size> seed_data{};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	randomEngine = std::mt19937{ seq };
}

template<std::size_t BOARD_LENGTH>
inline KnightTourist<BOARD_LENGTH>::KnightTourist(
	std::latch& closedToursToFind_,
	std::vector<ChessBoard<BOARD_LENGTH>>& closedTourBoards_,
	std::mutex& closedTourMutex_)
	: closedToursToFind(closedToursToFind_)
	, closedTourBoards(closedTourBoards_)
	, closedTourMutex(closedTourMutex_)
{
	initializeRandomEngine();
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::findClosedTour()
{
	bool foundClosedTour = false;
	while (!foundClosedTour)
	{
		board.clear();

		BoardLocation startingLocation(
			randomBoardIndexGenerator(randomEngine),
			randomBoardIndexGenerator(randomEngine));
		board.visitLocation(startingLocation, 1);

		auto currentLocation = startingLocation;
		std::size_t visitationOrder;
		for (visitationOrder = 2;
			visitationOrder <= board.getTotalLocations(); ++visitationOrder)
		{
			BoardLocation newValidLocation;
			if (!findNewValidLocation(currentLocation, newValidLocation))
			{
				break;
			}

			currentLocation = newValidLocation;
			board.visitLocation(newValidLocation, visitationOrder);
		}

		if (visitationOrder == board.getTotalLocations() + 1)
		{
			foundClosedTour = true;

			std::lock_guard<std::mutex> lock(closedTourMutex);
			closedTourBoards.push_back(std::move(board));

			closedToursToFind.count_down();

			break;
		}
	}
}

template<std::size_t BOARD_LENGTH>
bool KnightTourist<BOARD_LENGTH>::findNewValidLocation(
	BoardLocation const& startingLocation,
	BoardLocation& newValidLocation)
{
	std::array<bool, static_cast<std::size_t>(KnightMove::totalKnightMoves)>
		triedKnightMoves = {};

	bool foundNewValidLocation = false;

	do {
		auto knightMove = static_cast<KnightMove>(randomKnightMoveGenerator(
			randomEngine));

		newValidLocation = getNewLocation(startingLocation, knightMove);

		if (board.isLocationValid(newValidLocation) &&
			!board.hasLocationBeenVisited(newValidLocation))
		{
			foundNewValidLocation = true;
			break;
		}
		else
		{
			triedKnightMoves[static_cast<int>(knightMove)] = true;
		}

		if (std::all_of(triedKnightMoves.begin(),
			triedKnightMoves.end(), [](auto b) { return b == true; }))
		{
			break;
		}
	} while (true);

	return foundNewValidLocation;
}

template<std::size_t BOARD_LENGTH>
BoardLocation KnightTourist<BOARD_LENGTH>::getNewLocation(
	BoardLocation const& startingLocation, KnightMove knightMove) const
{
	BoardLocation newLocation;

	auto startingRowIndex = startingLocation.getRowIndex();
	auto startingColumnIndex = startingLocation.getColumnIndex();

	switch (knightMove)
	{
	case KnightMove::upperLeftHorizontal:
		newLocation.setRowIndex(startingRowIndex - 1);
		newLocation.setColumnIndex(startingColumnIndex - 2);
		break;
	case KnightMove::upperLeftVertical:
		newLocation.setRowIndex(startingRowIndex - 2);
		newLocation.setColumnIndex(startingColumnIndex - 1);
		break;
	case KnightMove::upperRightVertical:
		newLocation.setRowIndex(startingRowIndex - 2);
		newLocation.setColumnIndex(startingColumnIndex + 1);
		break;
	case KnightMove::upperRightHorizontal:
		newLocation.setRowIndex(startingRowIndex - 1);
		newLocation.setColumnIndex(startingColumnIndex + 2);
		break;
	case KnightMove::lowerLeftHorizontal:
		newLocation.setRowIndex(startingRowIndex + 1);
		newLocation.setColumnIndex(startingColumnIndex - 2);
		break;
	case KnightMove::lowerLeftVertical :
		newLocation.setRowIndex(startingRowIndex + 2);
		newLocation.setColumnIndex(startingColumnIndex - 1);
		break;
	case KnightMove::lowerRightVertical:
		newLocation.setRowIndex(startingRowIndex + 2);
		newLocation.setColumnIndex(startingColumnIndex + 1);
		break;
	case KnightMove::lowerRightHorizontal:
		newLocation.setRowIndex(startingRowIndex + 1);
		newLocation.setColumnIndex(startingColumnIndex + 2);
		break;
	default:
		std::cout << "ERROR: " << __FUNCTION__ << "Invalid Knight Move "
			<< static_cast<int>(knightMove) << std::endl;
	}

	return newLocation;
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::showTour() const
{
	board.dump();
}
