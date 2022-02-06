#ifndef QLOX_MATRIX_H
#define QLOX_MATRIX_H

#include <string>
#include <array>

struct Space {
	int x;
	int y;

	inline bool operator==(Space a) const {
		if(a.x==x && a.y== y)
			return true;
		else
			return false;
	}
};

class Matrix {
private:
	int numberOfColumns;
	int numberOfRows;
public:
	Matrix(const int numberOfColumns, const int numberOfRows);
	Space indexToCoordinates(int index);
	int coordinatesToLed(const Space& index);
};


#endif //QLOX_MATRIX_H
