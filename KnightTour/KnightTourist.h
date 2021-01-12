#pragma once

#include <random>
#include <algorithm>
#include <mutex>
#include <latch>
#include <vector>
#include <bitset>

#include "BoardLocation.h"
#include "ChessBoard.h"

template <std::size_t BOARD_LENGTH>
class KnightTourist
{
public:
	KnightTourist
	(
		  std::latch& fullToursToFind_
		, std::vector<ChessBoard<BOARD_LENGTH>>& fullTourBoards_
		, std::mutex& fullTourMutex_
		, bool const& stopFinding_
	);

	void findFullTours();
	void checkIfFullTour();
	void tourBoard();
	void showTour() const;

private:
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

	std::array<BoardLocation, static_cast<int>(KnightMove::totalKnightMoves)>
		knightMovesOffsets{ {
			  {-1, -2}
			, {-2, -1}
			, {-2, +1}
			, {-1, +2}
			, {+1, -2}
			, {+2, -1}
			, {+2, +1}
			, {+1, +2}
		} };

private:
	void initializeRandomEngine();
	BoardLocation getNewLocation(BoardLocation const& startingLocation,
		KnightMove knightMove) const;
	bool findNewValidLocation(BoardLocation const& startingLocation,
		BoardLocation& newValidLocation);
	BoardLocation generateRandomBoardLocation();

private:
	std::latch& fullToursLatch;
	std::vector<ChessBoard<BOARD_LENGTH>>& fullTourBoards;
	std::mutex& fullTourMutex;
	bool const& stopFinding;

	ChessBoard<BOARD_LENGTH> board;

	std::mt19937 randomEngine;
	std::uniform_int_distribution<> randomBoardIndexGenerator
		{ 1, BOARD_LENGTH };
	std::uniform_int_distribution<> randomKnightMoveGenerator
		{ 0, static_cast<int>(KnightMove::totalKnightMoves) - 1 };

	std::bitset<static_cast<std::size_t>(KnightMove::totalKnightMoves)>
		triedKnightMoves = {};

	std::size_t visitationOrder = 1;
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
KnightTourist<BOARD_LENGTH>::KnightTourist
(
	  std::latch& fullToursToFind_
	, std::vector<ChessBoard<BOARD_LENGTH>>& fullTourBoards_
	, std::mutex& fullTourMutex_
	, bool const& stopFinding_
)
	: fullToursLatch(fullToursToFind_)
	, fullTourBoards(fullTourBoards_)
	, fullTourMutex(fullTourMutex_)
	, stopFinding(stopFinding_)
{
	initializeRandomEngine();
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::findFullTours()
{
	while (!stopFinding)
	{
		board.clear();
		tourBoard();
		checkIfFullTour();
	}
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::checkIfFullTour()
{
	if (visitationOrder == board.getTotalLocations() + 1)
	{
		std::lock_guard<std::mutex> lock(fullTourMutex);
		fullTourBoards.push_back(std::move(board));
		fullToursLatch.count_down();
	}
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::tourBoard()
{
	BoardLocation startingLocation = generateRandomBoardLocation();
	board.visitLocation(startingLocation, 1);

	auto currentLocation = startingLocation;
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
}

template<std::size_t BOARD_LENGTH>
BoardLocation KnightTourist<BOARD_LENGTH>::generateRandomBoardLocation()
{
	return BoardLocation(
		randomBoardIndexGenerator(randomEngine),
		randomBoardIndexGenerator(randomEngine));
}

template<std::size_t BOARD_LENGTH>
bool KnightTourist<BOARD_LENGTH>::findNewValidLocation(
	BoardLocation const& startingLocation,
	BoardLocation& newValidLocation)
{
	triedKnightMoves.reset();
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
			triedKnightMoves.set(static_cast<int>(knightMove));
		}

		if (triedKnightMoves.all())
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

	auto const& knightMoveOffset = knightMovesOffsets[static_cast<int>(knightMove)];
	auto rowIndexOffset = knightMoveOffset.getRowIndex();
	auto columnIndexOffset = knightMoveOffset.getColumnIndex();

	newLocation.setRowIndex(startingRowIndex + rowIndexOffset);
	newLocation.setColumnIndex(startingColumnIndex + columnIndexOffset);

	return newLocation;
}

template<std::size_t BOARD_LENGTH>
void KnightTourist<BOARD_LENGTH>::showTour() const
{
	board.dump();
}
