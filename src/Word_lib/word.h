#ifndef QLOX_WORD_H
#define QLOX_WORD_H
#include <vector>
#include <string>

class Word {
private:
	int numberOfColumns;
	int lettersLength;
	std::string letters;
public:
	Word(const std::string& letters, const int numberOfColumns);
	struct std::vector<int> findSentence(const std::string& sentence);
};

#endif //QLOX_WORD_H
