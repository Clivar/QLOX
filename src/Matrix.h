#include <string>
#include <array>
#include <math.h>
#include <vector>
#include "FastLED.h"
#include <cstdio>

#pragma once

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

struct IndexWithColor
{
	CRGB color;
	int index;
};

template <size_t numLedsPerLetter = 1>
class Matrix
{
private:
	int numberOfColumns;
	int numberOfRows;

public:
	Matrix(const int numberOfColumns, const int numberOfRows);
	Coordinate indexToCoordinate(int index, const Coordinate &offset);
	std::array<int, numLedsPerLetter> coordinateToLed(const Coordinate &s);
};

template <size_t ledCountPerLetter>
Matrix<ledCountPerLetter>::Matrix(const int numberOfColumns, const int numberOfRows)
{
	this->numberOfColumns = numberOfColumns;
	this->numberOfRows = numberOfRows;
}

template <size_t numLedsPerLetter>
Coordinate Matrix<numLedsPerLetter>::indexToCoordinate(int index, const Coordinate &offset)
{
	int x = index % numberOfColumns;
	int y = floor((double)index / numberOfColumns);

	// Shift x and y according to offset
	x += offset.x;
	y += offset.y;

	return {x, y};
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