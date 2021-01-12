#pragma once

class BoardLocation
{
public:
	using Index = int;

	BoardLocation();
	BoardLocation(Index rowIndex_, Index columnIndex_);

	Index getRowIndex() const;
	Index getColumnIndex() const;
	void setRowIndex(Index rowIndex_);
	void setColumnIndex(Index columnIndex_);
	BoardLocation& operator+=(BoardLocation const& rhs);

private:
	Index rowIndex = 0;
	Index columnIndex = 0;
};

