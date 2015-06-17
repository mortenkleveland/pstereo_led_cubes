#include "FastLED.h"

#define NUM_LEDS 50
#define COLOR_ORDER RGB
#define BRIGHTNESS 200
#define FRAMES_PER_SECOND 60
#define DATA_PIN 6
#define COOLING 55
#define SPARKING 120

const int SENSOR_PIN = 2;
int sensorState = 0;
int prevSensorState = sensorState;
CRGB leds[NUM_LEDS];

void setup() { 
  delay(1000);
  pinMode(SENSOR_PIN, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  random16_add_entropy(random());
  sensorState = digitalRead(SENSOR_PIN);
  if (sensorState != prevSensorState) {
    //blinkLED(100);
    fire2012();
    FastLED.show();
  } else {
    setColor(100, 0, 0);
  }
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void blinkLED(int ms) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(ms);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(ms);
}

void fire2012() {
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for(int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for(int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if(random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for(int j = 0; j < NUM_LEDS; j++) {
      leds[j] = HeatColor(heat[j]);
  }
}

void setColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(green, red, blue); // R & G do for some reason have to be inverted
  }
  FastLED.show();
}


