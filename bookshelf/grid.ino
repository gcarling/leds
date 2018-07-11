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

void colorSide(CHSV innerColor, CHSV outerColor, int side) {
  LEDStrip mid;
  LEDStrip outer[2];

  switch (side) {
    case 0: 
      mid = vertStrips[0][1];
      outer[0] = horizStrips[0][0];
      outer[1] = horizStrips[0][1];
      break;
    case 1: 
      mid = horizStrips[1][1];
      outer[0] = vertStrips[0][2];
      outer[1] = vertStrips[1][2];
      break;
    case 2: 
      mid = vertStrips[1][1];
      outer[0] = horizStrips[2][0];
      outer[1] = horizStrips[2][1];
      break;
    case 3: 
      mid = horizStrips[1][0];
      outer[0] = vertStrips[0][0];
      outer[1] = vertStrips[1][0];
      break;
  }

  colorStrip(mid, innerColor);
  colorStrip(outer[0], outerColor);
  colorStrip(outer[1], outerColor);
}

void spinny(CHSV colors[], int iterations, bool darthMaul) {
  int offsetStart = random(4);

  // int isForwards = random(2) == 0;
  // if (!isForwards) {
  //   offsetStart = offsetStart * -1;
  // }

  for (int n = 0; n < iterations; n++) {
    for (int i = 0; i < 4; i++) {
      allToColor(CRGB::Black);
      int curInd = (i + offsetStart) % 4;
      colorSide(colors[0], colors[1], curInd);
      if (darthMaul) {
        int otherInd = (curInd + 2) % 4;
        colorSide(colors[0], colors[1], otherInd);
      }
      FastLED.show();
      delay(darthMaul ? 600 : 150);
    }
  }
}

// returns plus color
CHSV plusBox(CHSV plusColors[]) {
  CHSV testo[2] = {CHSV(0,0,0), CHSV(0,0,0)};
  complementPattern(testo);
  colorPlus(testo[0], CHSV(0, 0, 0));
  delay(600);
  colorPlus(CHSV(0, 0, 0), testo[1]);
  delay(600);

  plusColors[0] = testo[0];
  plusColors[1] = testo[1];
}

void boxStuff() {
  CHSV spinColors[] = {CHSV(0,0,0), CHSV(0,0,0)};
  plusBox(spinColors);
  spinny(spinColors, 5, false);
  plusBox(spinColors);
  spinny(spinColors, 2, true);
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
