#include <cstring>
#include <cstdio>
#include <cmath>
#include <iostream>
#include "word.h"
#include <string>


Word::Word(const std::string& letters, const int numberOfColumns) {
	this->letters = letters;
	this->lettersLength = letters.length();
	this->numberOfColumns = numberOfColumns;
}

struct std::vector<int> Word::findSentence(const std::string& sentence) {
	int sentenceLength = sentence.length();
	std::vector<int> result;

	std::vector<int> hits;
	int nextWordStartIndex = 0;
	for (int i = 0; i < lettersLength; i++) {
		if (nextWordStartIndex >= sentenceLength - 1) {
			break;
		}

		for (int j = nextWordStartIndex; j < sentenceLength; j++) {
			if (letters[i] == sentence[j]) {
#ifdef DEBUG
				printf("Matched (%c) on index %d\n", letters[i], i);
#endif
				hits.push_back(i);
				// We reached the end of the word if we are at the end of our words or we encounter a space in the next index
				if (j == sentenceLength - 1 || sentence[j + 1] == ' ') {
#ifdef DEBUG
					printf("Found final letter (%c) on index %d\n", letters[i], i);
#endif
					result.insert(result.end(), hits.begin(), hits.end());
					// Move to the next word by skipping a potential space
					nextWordStartIndex = j + 2;
					// We can also move up in letter indexes since words should never be right next to eachother.
					// i = i + 1;
					// Let's start looking for our next word by resetting the hitIndex
					hits.clear();
					break;
				}
				// Move to the next letter!
				i = i + 1;
				continue;
			} else if (hits.size() > 0) {
#ifdef DEBUG
				printf("Couldn't find next letter '%c'. Encountered '%c'\n", sentence[j], letters[i]);
#endif
				hits.clear();
				// The non matched letter could be a potential start
				if (letters[i] == sentence[nextWordStartIndex]) {
					// Rewind so it gets picked up
					i = i - 1;
				}
			}
			// When we reach here the letter didn't match
			// Start all the way over for the next letter so break
			break;
		}
	}
	return result;
}