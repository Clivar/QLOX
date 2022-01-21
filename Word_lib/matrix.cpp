//
// Created by Anthony on 20/01/2022.
//

#include <array>
#include "matrix.h"
#include <math.h>

Matrix::Matrix(const std::string& letters, const int numberOfColumns) {
	this->letters = letters;
	this->lettersLength = letters.length();
	this->numberOfColumns = numberOfColumns;
}

std::array<int, 2> Matrix::indexToCoordinates(int index) {
	index++;
	const int row = floor(index/numberOfColumns);
	const int column = (numberOfColumns % index) - 1;

	return { row, column };
}

int Matrix::coordinatesToLed(const std::array<int, 2>& index) {

}

