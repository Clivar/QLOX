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

RTC_DS3231 rtc;
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(256,33);

uint32_t wit = strip0.Color(255, 255, 255);

void initialize() {
	strip0.begin();//maak je object klaar
	strip0.setBrightness(20);//zet de helderheid wat lager (getal tussen 0 en brightness)
	strip0.show(); // toon de ingestelde kleuren (aangezien er nog geen kleuren zijn ingesteld, betekent dit hier: zet de display uit)
	rtc.begin();
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void drawSentence(const std::string sentence) {
	std::vector<Space> r = w.findSentence(sentence);
	for (int i = 0; i < r.size(); i++) {
		strip0.fill(wit, r[i].position, r[i].length);
		strip0.show();
	}
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

std::string timeToLiteral(DateTime now) {
	const int hour = now.hour() > 12 ? now.hour() - 12 : now.hour();
	const int minute = now.minute(); 
	if (minute == 0) {
		return numberToText(hour) + " UUR";
	}
	if (0 < minute && minute < 15) {
		return numberToText(minute) + " OVER " + numberToText(hour);
	}
	if (minute == 15) {
		return "KWART NA " + numberToText(hour);
	}
	if (15 < minute && minute < 30) {
		return numberToText(30-minute) + " VOOR HALF " + numberToText(hour+1);
	}
	if (minute == 30) {
		return "HALF " + numberToText(hour);
	}
	if (30 < minute && minute < 45) {
		return numberToText(minute-30) + " OVER HALF " + numberToText(hour+1);
	}
	if (minute == 45) {
		return "KWART VOOR " + numberToText(hour);
	}
	if (45 < minute) {
		return numberToText(60-minute) + " VOOR " + numberToText(hour+1);
	}
  #ifdef DEBUG
	Serial.println("Invalid minutes: " + minute);
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
	strip0.fill((0,0,0));
	const DateTime now = rtc.now();
	std::string timeInText = "HET IS " + timeToLiteral(now);
	drawSentence(timeInText.c_str());
	delay(10000);
}

