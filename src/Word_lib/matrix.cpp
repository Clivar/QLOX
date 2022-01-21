#include <array>
#include "matrix.h"
#include <math.h>

Matrix::Matrix(const int numberOfColumns, const int numberOfRows) {
	this->numberOfColumns = numberOfColumns;
	this->numberOfRows = numberOfRows;
}

Space Matrix::indexToCoordinates(int index) {
	index++;
	int row = floor(index/numberOfColumns);
	int column = (index % numberOfColumns) - 1;
	if (column == -1) {
		row--;
		column = numberOfColumns - 1;
	}

	return { column, row };
}

int Matrix::coordinatesToLed(const Space& s) {
	const int rowsFromBottom = numberOfRows - (s.y + 1);
	if (rowsFromBottom % 2 == 0) {
		return (rowsFromBottom * numberOfColumns) + s.x;
	}

	return (rowsFromBottom * numberOfColumns) + (numberOfColumns - s.x) - 1;
}

