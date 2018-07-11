#include <FastLED.h>

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
