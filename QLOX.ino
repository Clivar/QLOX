#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"
#include <iostream>
#include "Words_lib/word.h"

#define DEBUG
// #define RESET_TIME

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

RTC_DS3231 rtc;
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(256, 33);

uint32_t wit = strip0.Color(255, 255, 255);

void initialize() {
	strip0.begin();
	strip0.setBrightness(20);
	strip0.show();
	rtc.begin();
#ifdef RESET_TIME
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#endif
}

void drawSentence(const std::string sentence) {
	strip0.clear();
	std::vector<Space> r = w.findSentence(sentence);
	for (int i = 0; i < r.size(); i++) {
		strip0.fill(wit, r[i].position, r[i].length);
	}
	strip0.show();
}

std::string numberToText(int number) {
	switch(number) {
		case 0: return "TWAALF";
		case 1: return "EEN";
		case 2: return "TWEE";
		case 3: return "DRIE";
		case 4: return "VIER";
		case 5: return "VIJF";
		case 6: return "ZES";
		case 7: return "ZEVEN";
		case 8: return "ACHT";
		case 9: return "NEGEN";
		case 10: return "TIEN";
		case 11: return "ELF";
		case 12: return "TWAALF";
		case 13: return "DERTIEN";
		case 14: return "VEERTIEN";
	}
#ifdef DEBUG
	Serial.println("Unknown number: " + number);
#endif
	return "";
}

std::string timeToLiteral(int hour, int minutes) {
	hour = hour > 12 ? hour - 12 : hour;
	if (minutes == 0) {
		return numberToText(hour) + " UUR";
	}
	if (0 < minutes && minutes < 15) {
		return numberToText(minutes) + " OVER " + numberToText(hour);
	}
	if (minutes == 15) {
		return "KWART OVER " + numberToText(hour);
	}
	if (15 < minutes && minutes < 30) {
		return numberToText(30 - minutes) + " VOOR HALF " + numberToText(hour + 1);
	}
	if (minutes == 30) {
		return "HALF " + numberToText(hour);
	}
	if (30 < minutes && minutes < 45) {
		return numberToText(minutes - 30) + " OVER HALF " + numberToText(hour + 1);
	}
	if (minutes == 45) {
		return "KWART VOOR " + numberToText(hour);
	}
	if (45 < minutes) {
		return numberToText(60 - minutes) + " VOOR " + numberToText(hour + 1);
	}
#ifdef DEBUG
	Serial.println("Invalid minutes: " + minutes);
#endif
	return "";
}

void setup() {
#ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Debug mode active");
#endif
	initialize();
}

void loop() {
	const DateTime now = rtc.now();
	const int hour = now.hour();
	const int minutes = now.minute();

	std::string timeInText = "HET IS " + timeToLiteral(hour, minutes);
	drawSentence(timeInText.c_str());

	delay(10000);
}
