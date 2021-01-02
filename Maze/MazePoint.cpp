#include "MazePoint.h"

MazePoint::MazePoint(Index rowIndex_, Index columnIndex_)
	: rowIndex(rowIndex_), columnIndex(columnIndex_)
{
}

MazePoint::Index MazePoint::getRowIndex() const
{
	return rowIndex;
}

MazePoint::Index MazePoint::getColumnIndex() const
{
	return columnIndex;
}

bool MazePoint::operator==(MazePoint const& rhs) const
{
	return getRowIndex() == rhs.getRowIndex() &&
		getColumnIndex() == rhs.getColumnIndex();
}
