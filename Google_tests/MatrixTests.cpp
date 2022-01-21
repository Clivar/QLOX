#include "gtest/gtest.h"
#include "word.h"
#include "matrix.h"

class WordFixture : public ::testing::Test {

protected:
	virtual void SetUp()
	{
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
		word = new Word(letters, 16);
		matrix = new Matrix(letters, 16);
	}

	virtual void TearDown() {
		delete word;
		delete matrix;
	}

	Word * word;
	Matrix * matrix;
};

TEST_F(WordFixture, IndexToCoordinates){

}