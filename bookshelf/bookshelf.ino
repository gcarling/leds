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
  // complementPatternLoop(3);
  downwardsCradle();
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

void colorStrip(LEDStrip strip, CHSV color) {
  int offset = strip.start < strip.end ? 1 : -1;

  for (int cur = strip.start; cur != strip.end + offset; cur += offset) {
    leds[cur] = color;
  }
}

CHSV getComplement(CHSV color) {
  int newHue = (color.hue + 128) % 256;
  return CHSV(newHue, color.saturation, color.value);
}

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

void moveOnStripWithPhysics(int start, int end, bool shouldAccelerate) {
  int offset = start < end ? 1 : -1;

  int startValue = start + offset;

  for (int cur = startValue; cur != end + offset; cur += offset) {
    // if we should accelerate, start slow go faster. Otherwise do the opposite
    int distFromStart = abs(start - cur);
    int baseDelay = shouldAccelerate ? LEDS_PER_STRIP - distFromStart : distFromStart;
    delay((75 + baseDelay*baseDelay) / 4);

    // final iteration
    if (cur == end) {
      leds[cur] = leds[cur - offset];
      for (int inner = startValue; inner != end; inner += offset) {
        leds[inner] = CRGB::Black;
      }
    } else {
      for (int inner = cur; inner != startValue - offset; inner -= offset) {
        CHSV oldColor = rgb2hsv_approximate(leds[inner - offset]);
        leds[inner] = oldColor;

        int newValue = oldColor.value - 20;
        if (newValue < 0) {
          break;
        }

        leds[inner - offset] = CHSV(oldColor.hue, oldColor.saturation, newValue);
      }
    }


    FastLED.show();
  }

  // for (int cur = start + offset; cur != end + offset; cur += offset) {
  //   leds[cur] = CRGB::Black;
  // }
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
  leds[strip.end] = CHSV::Black;

  colorCradle(vertStart, vertEnd, newColor);
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

// A is plus color, B is background
void colorPlus(CHSV colorA, CHSV colorB) {
  LEDStrip strip;
  for (int row = 0; row < HORIZ_ROWS; row++){ 
    for (int col = 0; col < HORIZ_COLS; col++) {
      strip = horizStrips[row][col];
      CHSV color = row % 2 == 0 ? colorB : colorA;
      colorStrip(strip, color);
    }
  }

  for (int row = 0; row < VERT_ROWS; row++){ 
    for (int col = 0; col < VERT_COLS; col++) {
      strip = vertStrips[row][col];
      colorStrip(strip, colorA);
    }
  }
  FastLED.show();
}

// complement stuff
void complementPattern() {
  CHSV colorA = randomColor();
  CHSV colorB = getComplement(colorA);

  colorPlus(colorA, colorB);
  delay(1000);
  colorPlus(colorB, colorA);
  delay(1000);
}

void complementPatternLoop(int iterations) {
  for (int i = 0; i < iterations; i++) {
    complementPattern();
  }
}
