#include <math.h>

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN        9
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define LEDS_PER_STRIP  15
#define NUM_LEDS        90

int NUM_STRIPS = NUM_LEDS / LEDS_PER_STRIP;

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          85
#define FRAMES_PER_SECOND  120

void setup() {
//  Serial.begin(9600);
  
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
  
void loop() {
//  for (int i = 0; i < 5; i++) {
//    cradle(80);  
//  }
//  randomSampleLoop(3);
  allToColorInStrips();
  delay(800);
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

void cradle(int frameRate) {
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

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
