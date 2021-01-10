#pragma once

#include <array>
#include <iostream>

#include "BoardLocation.h"

template <std::size_t BOARD_LENGTH>
class ChessBoard
{
public:
	using LocationValue = int;

	void clear();
	void dump() const;
	bool isLocationValid(BoardLocation const& location) const;
	bool hasLocationBeenVisited(BoardLocation const& location) const;
	void visitLocation(BoardLocation const& location,
		LocationValue visitationOrder);
	constexpr std::size_t getTotalLocations() const;

private:
	void setLocationValue(BoardLocation const& location, LocationValue value);
	LocationValue getLocationValue(BoardLocation const& location) const;

	std::array<
		std::array<LocationValue, BOARD_LENGTH + 1>,
		BOARD_LENGTH + 1> grid = {};
};

template<std::size_t BOARD_LENGTH>
inline void ChessBoard<BOARD_LENGTH>::clear()
{
	grid = {};
}

template<std::size_t BOARD_LENGTH>
void ChessBoard<BOARD_LENGTH>::dump() const
{
	for (int i = 1; i <= BOARD_LENGTH; ++i)
	{
		for (int j = 1; j <= BOARD_LENGTH; ++j)
		{
			std::cout << grid[i][j] << ",";
		}
		std::cout << std::endl;
	}
}

template<std::size_t BOARD_LENGTH>
bool ChessBoard<BOARD_LENGTH>::isLocationValid
	(BoardLocation const& location) const
{
	auto rowIndex = location.getRowIndex();
	auto columnIndex = location.getColumnIndex();

	if (rowIndex <= 0 || rowIndex > BOARD_LENGTH)
	{
		return false;
	}

	if (columnIndex <= 0 || columnIndex > BOARD_LENGTH)
	{
		return false;
	}

	return true;
}

template<std::size_t BOARD_LENGTH>
inline void ChessBoard<BOARD_LENGTH>::setLocationValue(BoardLocation const& location,
	LocationValue value)
{
	grid[location.getRowIndex()][location.getColumnIndex()] = value;
}

template<std::size_t BOARD_LENGTH>
inline typename ChessBoard<BOARD_LENGTH>::LocationValue 
	ChessBoard<BOARD_LENGTH>::getLocationValue(BoardLocation const& location) const
{
	return grid[location.getRowIndex()][location.getColumnIndex()];
}

template<std::size_t BOARD_LENGTH>
inline bool ChessBoard<BOARD_LENGTH>::hasLocationBeenVisited(
	BoardLocation const& location) const
{
	return static_cast<bool>(getLocationValue(location));
}

template<std::size_t BOARD_LENGTH>
inline void ChessBoard<BOARD_LENGTH>::visitLocation(
	BoardLocation const& location, LocationValue visitationOrder)
{
	setLocationValue(location, visitationOrder);
}

template<std::size_t BOARD_LENGTH>
inline constexpr std::size_t ChessBoard<BOARD_LENGTH>::getTotalLocations() const
{
	return BOARD_LENGTH * BOARD_LENGTH;
}
