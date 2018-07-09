#include <math.h>

#include <FastLED.h>

#include "LEDStrip.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN        9
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define LEDS_PER_STRIP  15
#define NUM_LEDS        180
#define BRIGHTNESS      85

#define HORIZ_ROWS       3
#define HORIZ_COLS       2
#define VERT_ROWS        2
#define VERT_COLS        3

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
  {{59, 45}, {30, 44}},
  {{60, 74}, {89, 75}},
};

LEDStrip vertStrips[VERT_ROWS][VERT_COLS] = {
  {{179, 165}, {120, 134}, {119, 105}},
  {{150, 164}, {149, 135}, {90, 104}},
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
  boxStuff();
  downwardsCradle();
  randomSampleLoop(5);
  complementPatternLoop(2);
}

void allToColor(CRGB color) {
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

void colorStripRGB(LEDStrip strip, CRGB color) {
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

// puts a new color in this index, returns new color
CRGB updateColor(int ind) {
  CRGB newColor = getNewColor(leds[ind]);
  leds[ind] = newColor;
  FastLED.show();
  return newColor;
}

void downwardsCradle() {
  for (int r = 0; r < HORIZ_ROWS; r++) {
    for (int c = 0; c < HORIZ_COLS; c++) {
      cradle2D(r, c);
    }
  }
}

void cradle2D(int row, int col) {
  int frameRate = 80;
  // weird but just the way it works right now
  int midStart = vertStrips[0][1].start;
  int midEnd = vertStrips[1][1].start;

  // If we're on left, we change left pillar on end
  bool isLeft = col == 0;
  int sideStart;
  int sideEnd;
  if (isLeft) {
    sideStart = vertStrips[1][0].start;
    sideEnd = vertStrips[0][0].start;
  } else {
    sideStart = vertStrips[1][2].start;
    sideEnd = vertStrips[0][2].start;
  }


  CRGB startColor;
  if (leds[midStart] != CRGB(0, 0, 0)) {
    startColor = leds[midStart];
  } else {
    startColor = randomColor();
  }

  colorCradle(midStart, midEnd, startColor);

  // int stripRow = random(HORIZ_ROWS);
  // int stripCol = random(HORIZ_COLS);
  LEDStrip strip = horizStrips[row][col];

  leds[strip.end] = startColor;
  FastLED.show();
  moveOnStripWithPhysics(strip.end, strip.start, false);

  // Update color, handle strip
  CRGB newColor = updateColor(strip.start);
  colorCradle(sideStart, sideEnd, newColor);

  moveOnStripWithPhysics(strip.start, strip.end, true);

  leds[strip.end] = CRGB::Black;

  colorCradle(midStart, midEnd, newColor);
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
      CHSV color = col % 2 == 0 ? colorB : colorA;
      colorStrip(strip, color);
    }
  }
  FastLED.show();
}

// complement stuff
void complementPattern(CHSV finalColors[]) {
  CHSV colorA = randomColor();
  CHSV colorB = getComplement(colorA);

  colorPlus(colorA, colorB);
  delay(1000);
  colorPlus(colorB, colorA);
  delay(1000);

  if (sizeof(finalColors == 2)) {
    finalColors[0] = colorA;
    finalColors[1] = colorB;
  }
}

void complementPatternLoop(int iterations) {
  for (int i = 0; i < iterations; i++) {
    CHSV fake[] = {};
    complementPattern(fake);
  }
}

void spinny(CRGB startColor, int iterations, bool darthMaul) {
  LEDStrip centerStrips[4] = {horizStrips[1][0], vertStrips[0][1], horizStrips[1][1], vertStrips[1][1]};

  int offsetStart = random(4);
  int isForwards = random(1) == 0;
  if (!isForwards) {
    offsetStart = offsetStart * -1;
  }

  for (int n = 0; n < iterations; n++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        LEDStrip cur = centerStrips[j];
        colorStripRGB(cur, CRGB::Black);
      }
      int curInd = (i + offsetStart) % 4;
      colorStripRGB(centerStrips[curInd], startColor);
      if (darthMaul) {
        int otherInd = (curInd + 2) % 4;
        colorStripRGB(centerStrips[otherInd], startColor);
      }
      FastLED.show();
      delay(darthMaul ? 600 : 150);
    }
  }
}

// returns plus color
CHSV plusBox() {
  CHSV testo[2] = {CHSV(0, 0, 0), CHSV(0, 0, 0)};
  complementPattern(testo);
  colorPlus(testo[0], CHSV(0, 0, 0));
  delay(500);
  colorPlus(CHSV(0, 0, 0), testo[1]);
  delay(500);
  colorPlus(testo[0], CHSV(0, 0, 0));
  delay(500);
  colorPlus(CHSV(0, 0, 0), testo[1]);
  delay(500);

  return testo[0];
}

void boxStuff() {
  CHSV spinColor = plusBox();
  spinny(spinColor, 5, false);
  spinColor = plusBox();
  spinny(spinColor, 2, true);
}
