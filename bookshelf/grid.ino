#include <FastLED.h>

LEDStrip plusStrips[4] = {horizStrips[1][0], vertStrips[0][1], horizStrips[1][1], vertStrips[1][1]};

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

void randomPlus(int iterations) {
  for (int n = 0; n < iterations; n++) {
    for (int s = 0; s < 4; s++) {
      LEDStrip strip = plusStrips[s];
      randomStrip(strip);
    }
    FastLED.show();
    delay(200);
  }
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
  int offsetStart = random(4);
  // int isForwards = random(2) == 0;
  // if (!isForwards) {
  //   offsetStart = offsetStart * -1;
  // }

  for (int n = 0; n < iterations; n++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        LEDStrip cur = plusStrips[j];
        colorStripRGB(cur, CRGB::Black);
      }
      int curInd = (i + offsetStart) % 4;
      colorStripRGB(plusStrips[curInd], startColor);
      if (darthMaul) {
        int otherInd = (curInd + 2) % 4;
        colorStripRGB(plusStrips[otherInd], startColor);
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
  delay(600);
  colorPlus(CHSV(0, 0, 0), testo[1]);
  delay(600);

  return testo[0];
}

void boxStuff() {
  CHSV spinColor = plusBox();
  spinny(spinColor, 5, false);
  spinColor = plusBox();
  spinny(spinColor, 2, true);
}

void horizBars(int iterations) {
  allHorizToColor(CRGB::Black);
  FastLED.show();
  delay(200);
  CHSV color = randomColor();

  for (int n = 0; n < iterations; n++) {
    for (int r = 0; r < HORIZ_ROWS; r++) {
      allHorizToColor(CRGB::Black);
      for (int c = 0; c < HORIZ_COLS; c++) {
        colorStrip(horizStrips[r][c], color);
      }
      FastLED.show();
      delay(200);
    }
  }

  allHorizToColor(CRGB::Black);
  FastLED.show();
  delay(200);
}
