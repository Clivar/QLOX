#include <Arduino.h>
#include "Wireless.h"
#include "Api.h"

#include "FastLED.h"
#include <Wire.h>
#include "RTClib.h"
#include <vector>
#include <string>
#include <iostream>
#include "Word.h"
#include "Matrix.h"
#include "Sentences/DutchSentence.h"

// #define DEBUG
// #define RESET_TIME
#define DUTCH_SMALL_CLOCK

#ifdef DUTCH_SMALL_CLOCK
#define NUM_LEDS 256
#define NUM_LEDS_PER_LETTER 1
#define NUM_COLUMNS 16
#define NUM_ROWS 16

const std::string letters PROGMEM = "HETDAGNBPLUISZEG"
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
static class DutchSentence sentence = DutchSentence();
#endif

static class Word w = Word(letters, NUM_COLUMNS);
static class Matrix<1> m = Matrix<NUM_LEDS_PER_LETTER>(NUM_COLUMNS, NUM_ROWS);

static RTC_DS3231 rtc;
static CRGB leds[NUM_LEDS];

void drawSentence(std::string sentence, int red, int green, int blue);

void setup()
{
  Serial.begin(9600);
#ifdef DEBUG
  Serial.println("Debug mode active");
#endif
  Wireless::setup();
  Api::setup(&rtc);
  rtc.begin();
#ifdef RESET_TIME
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#endif
  FastLED.addLeds<NEOPIXEL, 33>(leds, NUM_LEDS);
}

void loop()
{
  EVERY_N_SECONDS(5)
  {
    fadeToBlack();
    state = stateMachine.next();
  }

  const ApiConfigurableOptions options = Api::GetConfigurableOptions();
  FastLED.setBrightness(options.Brightness);
  switch (state)
  {
  case State::WrittenTime:
  {
    const DateTime now = Api::getTime();
    std::string timeInText = sentence.getTime(now.hour(), now.minute());
    drawSentence(timeInText.c_str(), options.Red, options.Green, options.Blue);
    break;
  }
  case State::WrittenDate:
  {
    const DateTime now = Api::getTime();
    std::string dateInText = sentence.getDate(now.day(), now.month());
    drawSentence(dateInText.c_str(), options.Red, options.Green, options.Blue);
    break;
  }
  }
}

void drawSentence(std::string sentence, int red, int green, int blue)
{
  FastLED.clear();
  std::vector<int> r = w.findSentence(sentence);
  for (int i = 0; i < r.size(); i++)
  {
    Coordinate s = m.indexToCoordinate(r[i]);
    std::array<int, NUM_LEDS_PER_LETTER> ledIndexes = m.coordinateToLed(s);
    for (size_t i = 0; i < ledIndexes.size(); i++)
    {
      leds[ledIndexes[i]].setRGB(red, green, blue);
    }
  }
  FastLED.show();
}