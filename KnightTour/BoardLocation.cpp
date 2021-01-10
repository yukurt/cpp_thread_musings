#include "BoardLocation.h"

BoardLocation::BoardLocation()
{
}

BoardLocation::BoardLocation(Index rowIndex_, Index columnIndex_)
	: rowIndex(rowIndex_), columnIndex(columnIndex_)
{
}

BoardLocation::Index BoardLocation::getRowIndex() const
{
	return rowIndex;
}

BoardLocation::Index BoardLocation::getColumnIndex() const
{
	return columnIndex;
}

void BoardLocation::setRowIndex(BoardLocation::Index rowIndex_)
{
	rowIndex = rowIndex_;
}

void BoardLocation::setColumnIndex(BoardLocation::Index columnIndex_)
{
	columnIndex = columnIndex_;
}
