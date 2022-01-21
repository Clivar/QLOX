//
// Created by Anthony on 20/01/2022.
//

#ifndef QLOX_MATRIX_H
#define QLOX_MATRIX_H


#include <string>
#include <array>

struct Space {
	int row;
	int column;
};

class Matrix {
private:
	int numberOfColumns;
	int lettersLength;
	std::string letters;
public:
	Matrix(const std::string& letters, const int numberOfColumns);
	std::array<int, 2> indexToCoordinates(int index);
	int coordinatesToLed(const std::array<int, 2>& index);
};


#endif //QLOX_MATRIX_H
