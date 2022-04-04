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

// Specify what preset to use
#define PROTOTYPE2
#include "Presets.cpp"

// #define DEBUG
// #define RESET_TIME

const PROGMEM std::string letters = LETTERSEQUENCE

#ifdef DUTCH_SMALL_CLOCK
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
void drawRainAnimation(const CRGB &color);
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
  FastLED.addLeds<NEOPIXEL, 26>(leds, NUM_LEDS);
}

void loop()
{
  EVERY_N_SECONDS(10)
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
  case State::Rain:
  {
    drawRainAnimation(color);
    break;
  }
  case State::Rainbow:
  {
    static uint8_t hue = 0;
    EVERY_N_MILLISECONDS(20) { hue++; }
    fill_rainbow(leds, NUM_LEDS, hue, 7);
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
  FastLED.clear();
}

uint16_t XY(uint8_t x, uint8_t y)
{
  return (y * NUM_COLUMNS + x);
}

// Based on: Digital Rain implementation
// Yaroslaw Turbin 24.08.2020
// https://vk.com/ldirko
// https://www.reddit.com/user/ldirko/
void drawRainAnimation(const CRGB &color)
{
  static byte rain[NUM_LEDS];
  static bool init = true;
  static byte counter = 1;
  static int speed = 1;
  if (init)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (random8(20) == 0)
      {
        rain[i] = 1; // random8(20) number of dots. decrease for more dots
      }
      else
      {
        rain[i] = 0;
      }
    }
    init = false;
  }

  EVERY_N_MILLISECONDS(80)
  {
    for (byte i = 0; i < NUM_COLUMNS; i++)
    {
      for (byte j = 0; j < NUM_ROWS; j++)
      {
        byte layer = rain[((j + speed + random8(2) + NUM_ROWS) % NUM_ROWS) * NUM_COLUMNS + i];
        if (layer)
        {
          const uint8_t x = (NUM_COLUMNS - 1) - i;
          const uint8_t y = (NUM_ROWS - 1) - j;
          turnOnLed(m.coordinateToLed({x, y}), color);
        }
      }
    }

    speed++;
    fadeToBlackBy(leds, NUM_LEDS, 70);
    blurRows(leds, NUM_COLUMNS, NUM_ROWS, 16);
    FastLED.show();
  }

  EVERY_N_MILLISECONDS(15)
  {
    int rand1 = random16(NUM_LEDS);
    int rand2 = random16(NUM_LEDS);
    if ((rain[rand1] == 1) && (rain[rand2] == 0))
    {
      rain[rand1] = 0;
      rain[rand2] = 1;
    }
  }
}