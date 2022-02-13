#ifndef QLOX_MATRIX_H
#define QLOX_MATRIX_H

#include <string>
#include <array>

struct Coordinate
{
	int x;
	int y;

	inline bool operator==(Coordinate a) const
	{
		if (a.x == x && a.y == y)
			return true;
		else
			return false;
	}
};

struct LedIndexes
{
	int *leds;
	int length;
};

template <size_t numLedsPerLetter = 1>
class Matrix
{
private:
	int numberOfColumns;
	int numberOfRows;

public:
	Matrix(const int numberOfColumns, const int numberOfRows);
	Coordinate indexToCoordinate(int index);
	std::array<int, numLedsPerLetter> coordinateToLed(const Coordinate &s);
};

#endif // QLOX_MATRIX_H

template <size_t ledCountPerLetter>
Matrix<ledCountPerLetter>::Matrix(const int numberOfColumns, const int numberOfRows)
{
	this->numberOfColumns = numberOfColumns;
	this->numberOfRows = numberOfRows;
}

template <size_t numLedsPerLetter>
Coordinate Matrix<numLedsPerLetter>::indexToCoordinate(int index)
{
	index++;
	int row = floor(index / numberOfColumns);
	int column = (index % numberOfColumns) - 1;
	if (column == -1)
	{
		row--;
		column = numberOfColumns - 1;
	}

	return {column, row};
}

template <size_t numLedsPerLetter>
std::array<int, numLedsPerLetter> Matrix<numLedsPerLetter>::coordinateToLed(const Coordinate &s)
{
	std::array<int, numLedsPerLetter> result;
	const int rowsFromBottom = numberOfRows - (s.y + 1);
	if (rowsFromBottom % 2 == 0)
	{
		for (size_t i = 0; i < numLedsPerLetter; i++)
		{
			result[i] = (rowsFromBottom * numberOfColumns) + s.x + i;
		}

		return result;
	}

	for (size_t i = 0; i < numLedsPerLetter; i++)
	{
		result[i] = (rowsFromBottom * numberOfColumns) + (numberOfColumns - s.x) - 1 - i;
	}

	return result;
}