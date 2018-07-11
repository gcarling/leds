#include <FastLED.h>

void allToColor(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void allHorizToColor(CRGB color) {
  for (int r = 0; r < HORIZ_ROWS; r++) {
    for (int c = 0; c < HORIZ_COLS; c++) {
      colorStripRGB(horizStrips[r][c], color);
    }
  }
  FastLED.show();
}

void allVertToColor(CRGB color) {
  for (int r = 0; r < VERT_ROWS; r++) {
    for (int c = 0; c < VERT_COLS; c++) {
      colorStripRGB(vertStrips[r][c], color);
    }
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
