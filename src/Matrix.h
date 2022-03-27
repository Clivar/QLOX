#ifndef QLOX_MATRIX_H
#define QLOX_MATRIX_H

#include <string>
#include <array>
#include <math.h>

struct Coordinate
{
	unsigned short x;
	unsigned short y;

	inline bool operator==(Coordinate a) const
	{
		if (a.x == x && a.y == y)
			return true;
		else
			return false;
	}
};

struct ShapeColor
{
	unsigned short r;
	unsigned short g;
	unsigned short b;
};

struct IndexWithColor : ShapeColor
{
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
	std::vector<IndexWithColor> shapeToLed(std::vector<ShapeColor> &coloredShape, Coordinate &coordinateOfLeftUpperCorner, unsigned short numberOfColumns);
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

template <size_t numLedsPerLetter>
std::vector<IndexWithColor> Matrix<numLedsPerLetter>::shapeToLed(std::vector<ShapeColor> &coloredShape, Coordinate &coordinateOfLeftUpperCorner, unsigned short numberOfColumns)
{
	std::vector<IndexWithColor> result;
	for (size_t i = 0; i < coloredShape.size(); i++)
	{
		IndexWithColor c;
		c.r = coloredShape[i].r;
		c.g = coloredShape[i].g;
		c.b = coloredShape[i].b;

		Coordinate c;
		c.x = (numberOfColumns - i) % numberOfColumns;
		c.y = floor((double)i / numberOfColumns);

		// Shift x and y according to coordinateOfLeftUpperCorner
		c.x += coordinateOfLeftUpperCorner.x;
		c.y += coordinateOfLeftUpperCorner.y;

		const std::array<int, numLedsPerLetter> indexes = coordinateToLed(c);
		for (size_t j = 0; j < indexes.size(); j++)
		{
			IndexWithColor indexWithColor = c;
			indexWithColor.index = indexes[j];
			result.push_back(indexWithColor);
		}
	}

	return result;
}