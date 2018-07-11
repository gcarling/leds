#include <FastLED.h>

void colorStrip(LEDStrip strip, CHSV color) {
  int offset = strip.start < strip.end ? 1 : -1;

  for (int cur = strip.start; cur != strip.end + offset; cur += offset) {
    leds[cur] = color;
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
