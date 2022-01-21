
#ifndef ARDUINO
#include <iostream>
#include <vector>
#include <string>
#include "src/Word_lib/word.h"
#include "src/Word_lib/matrix.h"

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

static class Word w = Word(letters, 16);
static class Matrix m = Matrix(16, 16);

int main()
{
	m.indexToCoordinates(0);
	std::cout << "Hello, World!" << std::endl;
	return 0;
}
#endif