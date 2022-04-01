#include <Arduino.h>
#include "Wireless.h"
#include "Api.h"

#include "FastLED.h"
#include <Wire.h>
#include "RTClib.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "Word.h"
#include "Matrix.h"
#include "Sentences/DutchSentence.h"
#include "StateMachine.h"
#include "DigitalNumbers.h"

// #define DEBUG
// #define RESET_TIME
#define DUTCH_SMALL_CLOCK

#ifdef DUTCH_SMALL_CLOCK
#define NUM_LEDS 256
#define NUM_LEDS_PER_LETTER 1
#define NUM_COLUMNS 16
#define NUM_ROWS 16

const PROGMEM std::string letters = "HETDAGNBPLUISZEG"
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
static class Matrix<NUM_LEDS_PER_LETTER> m = Matrix<NUM_LEDS_PER_LETTER>(NUM_COLUMNS, NUM_ROWS);
static class StateMachine stateMachine;
static State state;

static RTC_DS3231 rtc;
static CRGB leds[NUM_LEDS];

void drawSentence(const std::string &sentence, const CRGB &color);
void drawNumber(uint8_t number, Coordinate offset, const CRGB &color);
void drawSecondIndicator(const CRGB &color);
void fadeToBlack();

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

  const CRGB color = CRGB(options.Red, options.Green, options.Blue);
  switch (state)
  {
  case State::WrittenTime:
  {
    const DateTime now = Api::getTime();
    std::string timeInText = sentence.getTime(now.hour(), now.minute());
    FastLED.clear();
    drawSentence(timeInText, color);
    FastLED.show();
    break;
  }
  case State::WrittenDate:
  {
    const DateTime now = Api::getTime();
    std::string dateInText = sentence.getDate(now.day(), now.month());
    FastLED.clear();
    drawSentence(dateInText, color);
    FastLED.show();
    break;
  }
  case State::DigitalTime:
  {
    const DateTime now = Api::getTime();
    FastLED.clear();
    drawNumber(now.hour() / 10, {2, 0}, color);
    drawNumber(now.hour() % 10, {9, 0}, color);
    drawSecondIndicator(color);
    drawNumber(now.minute() / 10, {2, 9}, color);
    drawNumber(now.minute() % 10, {9, 9}, color);
    FastLED.show();
    break;
  }
  }
}

void turnOnLed(const std::array<int, NUM_LEDS_PER_LETTER> &ledIndexes, const CRGB &color)
{
  for (size_t i = 0; i < ledIndexes.size(); i++)
  {
    leds[ledIndexes[i]] = color;
  }
}

void drawSecondIndicator(const CRGB &color)
{
  static bool turnItOn = false;
  EVERY_N_MILLISECONDS(1000)
  {
    turnItOn = !turnItOn;
  }

  if (!turnItOn)
    return;

  turnOnLed(m.coordinateToLed({7, 7}), color);
  turnOnLed(m.coordinateToLed({8, 7}), color);
  turnOnLed(m.coordinateToLed({7, 8}), color);
  turnOnLed(m.coordinateToLed({8, 8}), color);
}

void drawNumber(uint8_t number, Coordinate offset, const CRGB &color)
{
  // Number of leds is not important here, so we pick '1' to keep it simple
  // Use a smaller matrix to determine positions of number indexes
  Matrix<1> numberMatrix = Matrix<1>(5, 7);
  std::map<uint8_t, std::array<uint8_t, 35>> map{
      {0, zero},
      {1, one},
      {2, two},
      {3, three},
      {4, four},
      {5, five},
      {6, six},
      {7, seven},
      {8, eight},
      {9, nine},
  };

  const std::array<uint8_t, 35> a = map[number];

  for (size_t i = 0; i < a.size(); i++)
  {
    Coordinate c = numberMatrix.indexToCoordinate(i, offset);
    const std::array<int, NUM_LEDS_PER_LETTER> ledIndexes = m.coordinateToLed(c);
    if (a[i] > 0)
    {
      turnOnLed(ledIndexes, color);
    }
  }
}

void drawSentence(const std::string &sentence, const CRGB &color)
{
  std::vector<int> r = w.findSentence(sentence);
  for (int i = 0; i < r.size(); i++)
  {
    Coordinate s = m.indexToCoordinate(r[i], {0, 0});
    std::array<int, NUM_LEDS_PER_LETTER> ledIndexes = m.coordinateToLed(s);
    turnOnLed(ledIndexes, color);
  }
}

void fadeToBlack()
{
  for (int fade = 0; fade <= 50; fade++)
  {
    fadeToBlackBy(leds, NUM_LEDS, fade);
    FastLED.show();
    delay(30);
  }
}