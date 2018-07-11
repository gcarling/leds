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
  horizBars(3);
  squareRotate(3);
  complementPattern();
  randomPlus(10);
  allToColorInStripsLoop(3);
  downwardsCradle();
  horizBars(3);
}
