#include <iostream>
#include "Words_lib/word.h"

#ifndef ARDUINO
const std::string letters = "HETDAGNBPLUISZEG"
				 "RALKISTEENZESELF"
				 "DRIEVIERVIJFTWEE"
				 "ZEVENACHTNEGENEN"
				 "DERTIENVEERTIENY"
				 "TWAALFHKWARTSUUR"
				 "TWINTIGMOVERVOOR"
				 "HALFXEENTWEEDRIE"
				 "VIERVIJFZESZEVEN"
				 "ACHTNEGENTIENELF"
				 "TWAALFDERTIGDMEI"
				 "FEBRUARIJUNIJULI"
				 "XAPRILGTAUGUSTUS"
				 "SEPTEMBERJANUARI"
				 "TRMAARTWOKTOBERK"
				 "NOVEMBERDECEMBER";
static class Word word = Word(letters, 16);

void drawSentence(const std::string sentence);
int main() {
	drawSentence("HET IS");
	drawSentence("HET IS");
	return 0;
}

void drawSentence(const std::string sentence) {
	std::vector<Space> r = word.findSentence(sentence);
	for (int i = 0; i < r.size(); i++) {
		printf("%d", r[i].position);
		printf("%d", r[i].length);
	}
}
#endif
