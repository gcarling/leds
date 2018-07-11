#include <FastLED.h>

bool stripGoesForwards(LEDStrip strip) {
  // its forwards if end > start
  return strip.end > strip.start;
}

void colorStrip(LEDStrip strip, CHSV color) {
  int offset = strip.start < strip.end ? 1 : -1;

  for (int cur = strip.start; cur != strip.end + offset; cur += offset) {
    leds[cur] = color;
  }
}

void colorStrips(LEDStrip strips[], int len, CHSV color) {
  for (int i = 0; i < len; i++) {
    colorStrip(strips[i], color);
  }
}

void randomStrip(LEDStrip strip) {
  int offset = strip.start < strip.end ? 1 : -1;

  for (int cur = strip.start; cur != strip.end + offset; cur += offset) {
    leds[cur] = randomColor();
  }
}

void colorStripRGB(LEDStrip strip, CRGB color) {
  int offset = strip.start < strip.end ? 1 : -1;

  for (int cur = strip.start; cur != strip.end + offset; cur += offset) {
    leds[cur] = color;
  }
}
