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
		matrix = new Matrix(16, 16);
	}

	virtual void TearDown() {
		delete word;
		delete matrix;
	}

	Word * word;
	Matrix * matrix;
};

TEST_F(WordFixture, IndexToCoordinates) {
	EXPECT_EQ(matrix->indexToCoordinates(0), (Space { 0, 0 }));
	EXPECT_EQ(matrix->indexToCoordinates(16), (Space { 0, 1 }));
}

TEST_F(WordFixture, SingleWordOnFirstRow){
	std::vector<int> result = word->findSentence("HET IS EEN OVER HALF EEN");
	EXPECT_EQ(0, result[0]);
	EXPECT_EQ(1, result[1]);
	EXPECT_EQ(2, result[2]);
}

TEST_F(WordFixture, FoundTheWholeSentence){
	std::vector<int> result = word->findSentence("HET IS NEGEN UUR");
	for (int i = 0; i < result.size(); ++i) {
		Space s = matrix->indexToCoordinates(result[i]);
		printf("x: %d y: %d (index: %d)", s.x, s.y, result[i]);
		int led = matrix->coordinatesToLed(s);
		printf(" led index: %d\n", led);
	}
	EXPECT_EQ(13, result.size());
}

TEST_F(WordFixture, LedOnLastRow){
	EXPECT_EQ(matrix->coordinatesToLed(Space { 5, 15 }), 5);
}

TEST_F(WordFixture, LedOnOneButLastRow){
	EXPECT_EQ(matrix->coordinatesToLed(Space { 5, 14 }), 16+16-5);
}