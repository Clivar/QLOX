#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"
#include <iostream>
#include "word.h"

#define DEBUG

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

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(256,33);
uint32_t wit = strip0.Color(255, 255, 255);

void initialize() {
	strip0.begin();//maak je object klaar
	strip0.setBrightness(20);//zet de helderheid wat lager (getal tussen 0 en brightness)
	strip0.show(); // toon de ingestelde kleuren (aangezien er nog geen kleuren zijn ingesteld, betekent dit hier: zet de display uit)
}

void drawSentence(const std::string sentence) {
	std::vector<Space> r = w.findSentence(sentence);
	for (int i = 0; i < r.size(); i++) {
		strip0.fill(wit, r[i].position, r[i].length);
		strip0.show();
	}
}

void setup() {
	#ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Debug mode active");
	#endif
	initialize();
	drawSentence("HET IS NEGEN UUR");
}

void loop() {
	// put your main code here, to run repeatedly:
}