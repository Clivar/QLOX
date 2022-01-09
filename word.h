//
// Created by Anthony on 8/01/2022.
//

#ifndef QLOX_WORD_H
#define QLOX_WORD_H
#include <vector>

struct Space {
	int position;
	int length;
};

class Word {
private:
	int numberOfColumns;
	int lettersLength;
	std::string letters;
	int convertToLedIndex(int index, int length) const;

public:
	Word(const std::string letters, const int numberOfColumns);
	struct std::vector<Space> findSentence(const std::string sentence);
};

#endif //QLOX_WORD_H
