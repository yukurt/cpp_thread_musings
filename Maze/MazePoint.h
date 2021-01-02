#pragma once
class MazePoint
{
public:
	using Index = int;

public:
	MazePoint(Index rowIndex_, Index columnIndex_);
	Index getRowIndex() const;
	Index getColumnIndex() const;
	bool operator==(MazePoint const& rhs) const;

private:
	int rowIndex = 0;
	int columnIndex = 0;
};

