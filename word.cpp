//
// Created by Anthony on 8/01/2022.
//
#include <cstring>
#include <cstdio>
#include <cmath>
#include <iostream>
#include "word.h"

Word::Word(const std::string letters, const int numberOfColumns) {
	this->letters = letters;
	this->lettersLength = letters.length();
	this->numberOfColumns = numberOfColumns;
}

struct std::vector<Space> Word::findSentence(const std::string sentence) {
	int sentenceLength = sentence.length();
	std::vector<Space> result;

	int hitIndex = -1;
	int nextWordStartIndex = 0;
	for (int i = 0; i < lettersLength; i++) {
		if (nextWordStartIndex >= sentenceLength - 1) {
			break;
		}

		for (int j = nextWordStartIndex; j < sentenceLength; j++) {
			const int hitLength = i - hitIndex + 1;
			const int wordLength = j - nextWordStartIndex + 1;
			// We reached the end of the word if we are at the end of our words or we encounter a space in the next index
			const int endOfWord = j == sentenceLength - 1 || sentence[j + 1] == ' ';
			if (letters[i] == sentence[j] && hitIndex < 0) {
#ifdef DEBUG
				printf("Found potential start\n");
#endif
				hitIndex = i;
			}
			// Keep holding the hitindex until we finish our word
			if (endOfWord && letters[i] == sentence[j] && hitIndex >= 0 && hitLength == wordLength) {
#ifdef DEBUG
				printf("Found final letter (%c) on index %d\n", letters[i], i);
#endif
				struct Space r = { convertToLedIndex(hitIndex, hitLength), hitLength};
				result.push_back(r);
				// Move to the next word by skipping a potential space
				nextWordStartIndex = j + 2;
				// We can also move up in letter indexes since words should never be right next to eachother.
				// i = i + 1;
				// Let's start looking for our next word by resetting the hitIndex
				hitIndex = -1;
				break;
				// No hit? Reset it and start over!
			} else if (letters[i] != sentence[j]) {
#ifdef DEBUG
				printf("Couldn't find %c. Encountered %c\n", sentence[j], letters[i]);
#endif
				hitIndex = -1;
				// The non matched letter could be a potential start
				if (letters[i] == sentence[nextWordStartIndex]) {
					// Rewind so it gets picked up
					i = i - 1;
				}
				break;
			}
#ifdef DEBUG
			printf("Matched (%c) on index %d\n", letters[i], i);
#endif
			// Move to the next letter!
			i = i + 1;
		}
	}
	return result;
}

int Word::convertToLedIndex(int index, int length) const {
	const int invertedIndex = lettersLength - index;
	const int rowFromBottom = floor((invertedIndex - 1) / numberOfColumns);
	const int blocksFromBottom = rowFromBottom * numberOfColumns;
	const int remainder = invertedIndex % numberOfColumns;
	// Count from left
	if (rowFromBottom % 2 == 0) {
		if (remainder > 0) {
			return blocksFromBottom + (numberOfColumns - remainder);
		} else {
			return blocksFromBottom;
		}
	}
		// Count from right
	else {
		if (remainder > 0) {
			return blocksFromBottom + remainder - length;
		} else {
			return blocksFromBottom + numberOfColumns - length;
		}
	}
}