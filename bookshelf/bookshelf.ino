#include <math.h>

#include <FastLED.h>

#include "LEDStrip.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN        9
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define LEDS_PER_STRIP  15
#define NUM_LEDS        120
#define BRIGHTNESS      85

#define HORIZ_ROWS       3
#define HORIZ_COLS       2
#define VERT_ROWS        2
#define VERT_COLS        1

int NUM_STRIPS = NUM_LEDS / LEDS_PER_STRIP;

CRGB leds[NUM_LEDS];

bool stripGoesForwards(LEDStrip strip) {
  // quick assertion that the length is correct
  // assert(abs(start - end) == LEDS_PER_STRIP - 1)
  // its forwards if end > start
  return strip.end > strip.start;
}

LEDStrip horizStrips[HORIZ_ROWS][HORIZ_COLS] = {
  {{0, 14}, {29, 15}},
  {{30, 44}, {59, 45}},
  {{60, 74}, {89, 75}},
};

LEDStrip vertStrips[VERT_ROWS][VERT_COLS] = {
  {{90, 104}},
  {{119, 105}},
};

void setup() {
//  Serial.begin(9600);
  
  delay(1000); // 1 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  downwardsCradle();
  randomSampleLoop(3);
}

void allToColor(CHSV color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void allToColorInStrips() {
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    CHSV color = randomColor();
    for (int i = strip * LEDS_PER_STRIP; i < (strip + 1) * LEDS_PER_STRIP; i++) {
      leds[i] = color;
    }
  }
  FastLED.show();
}

// helpers
CHSV randomColor() {
  int color = random(256);

  return CHSV(color, 255, BRIGHTNESS);
}

CRGB getNewColor(CRGB oldColor) {
  CRGB newColor = randomColor();
  while (oldColor == newColor) {
    newColor = randomColor();   
  }
  return newColor;
}

// cradle
void colorCradle(int cradleStart, int cradleEnd, CRGB newColor) {
  for (int i = cradleStart; i <= cradleEnd; i++) {
    leds[i] = newColor;
  }
  FastLED.show();
}

void moveOnLine(int start, int end, int frameRate) {
  int offset = start < end ? 1 : -1;

  for (int cur = start + offset; cur != end + offset; cur += offset) {
    delay(frameRate);
    leds[cur] = leds[cur - offset];
    leds[cur - offset] = CRGB(0, 0, 0);
    FastLED.show();
  }
}

void moveOnStripWithPhysics(int start, int end, bool shouldAccelerate) {
  int offset = start < end ? 1 : -1;

  for (int cur = start + offset; cur != end + offset; cur += offset) {
    // if we should accelerate, start slow go faster. Otherwise do the opposite
    int distFromStart = abs(start - cur);
    int baseDelay = shouldAccelerate ? LEDS_PER_STRIP - distFromStart : distFromStart;
    delay((75 + baseDelay*baseDelay) / 4);
    leds[cur] = leds[cur - offset];
    leds[cur - offset] = CRGB(0, 0, 0);
    FastLED.show();
  }
}

// puts a new color in this index
void updateColor(int ind) {
  leds[ind] = getNewColor(leds[ind]);
  FastLED.show();
}

void downwardsCradle() {
  for (int r = 0; r < HORIZ_ROWS; r++) {
    bool reverse = r % 2 != 0;
    if (!reverse) {
      for (int c = 0; c < HORIZ_COLS; c++) {
        cradle2D(r, c);
      }
    } else {
      for (int c = HORIZ_COLS - 1; c >= 0; c--) {
        cradle2D(r, c);
      }
    }
  }
}

void cradle2D(int row, int col) {
  int frameRate = 80;
  // weird but just the way it works right now
  int vertStart = vertStrips[0][0].start;
  int vertEnd = vertStrips[1][0].start;

  CRGB startColor;
  if (leds[vertStart] != CRGB(0, 0, 0)) {
    startColor = leds[vertStart];
  } else {
    startColor = randomColor();
  }

  colorCradle(vertStart, vertEnd, startColor);

  // int stripRow = random(HORIZ_ROWS);
  // int stripCol = random(HORIZ_COLS);
  LEDStrip strip = horizStrips[row][col];

  leds[strip.end] = startColor;
  FastLED.show();
  moveOnStripWithPhysics(strip.end, strip.start, false);
  updateColor(strip.start);
  moveOnStripWithPhysics(strip.start, strip.end, true);

  CRGB newColor = leds[strip.end];
  leds[strip.end] = CRGB::Black;

  colorCradle(vertStart, vertEnd, newColor);
}

void cradle1D(int frameRate) {
  // cradle stuff
  int cradleSize = ceil(NUM_LEDS / 3);
  
  int cradleStart = floor(NUM_LEDS / 3);
  int cradleEnd = NUM_LEDS - floor(NUM_LEDS / 3) - 1;

  CRGB startColor;
  if (leds[cradleStart] != CRGB(0, 0, 0)) {
    startColor = leds[cradleStart];
  } else {
    startColor = randomColor();
  }
  
  colorCradle(cradleStart, cradleEnd, startColor);

  moveOnLine(cradleEnd, NUM_LEDS - 1, frameRate);
  leds[NUM_LEDS - 1] = getNewColor(leds[NUM_LEDS - 1]);
  FastLED.show();
  moveOnLine(NUM_LEDS - 1, cradleEnd, frameRate);
  // color cradle
  colorCradle(cradleStart, cradleEnd, leds[cradleEnd]);
  moveOnLine(cradleStart, 0, frameRate);
  leds[0] = getNewColor(leds[0]);
  FastLED.show();
  moveOnLine(0, cradleStart, frameRate);
  colorCradle(cradleStart, cradleEnd, leds[cradleStart]);
}

// random colors
void randomSample() {
for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = randomColor();
  } 
}

void randomSampleLoop(int numLoops) {
  for (int i = 0; i < numLoops; i++) {
    randomSample();
    FastLED.show();
    delay(300);
  }
}
