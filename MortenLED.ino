#include "FastLED.h"

#define NUM_LEDS 50
#define COLOR_ORDER RGB
#define BRIGHTNESS 100
#define FRAMES_PER_SECOND 60
#define DATA_PIN 6
#define COOLING 55
#define SPARKING 120

const int SENSOR_PIN = 2;
const int ANALOG_IN = A0;
int sensorState = 0;
int prevSensorState = sensorState;
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  delay(1000);
  pinMode(SENSOR_PIN, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  random16_add_entropy(random());
  sensorState = digitalRead(SENSOR_PIN);
  int potmeterValue = analogRead(ANALOG_IN);
  Serial.println(potmeterValue/4);

  setColor(0, 100, 0);
  FastLED.show();
  delay(1000);
  fade(0, 100, 0, 100, 0, 0, 500);
  setColor(100, 0, 0);
  FastLED.show();
  delay(1000);
  fade(100, 0, 0, 0, 100, 0, 500);
  
//  if (sensorState != prevSensorState) {
//    if(sensorState == HIGH) {
//      fade(0, 100, 0, 100, 0, 0, 500);
//      FastLED.show();
//      setColor(100, 0, 0);
//      FastLED.show();
//    }
//    //blinkLED(100);
//    /*
//    fire2012();
//    for (int i = 0; i < NUM_LEDS; i++) {
//      leds[i] += CRGB(0, 20, 40);
//    }
//    */
//    
//    FastLED.show();
//  } else {
//    fire2012();
//
//    //setColor(0, 100, 0);
//    FastLED.show();
//  }
//  prevSensorState = sensorState;
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
  for(int k = NUM_LEDS - 1; k >= 2; k--) {
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
    leds[i].setRGB(red, green, blue);
  }
  FastLED.show();
}

void fade(int fromRed, int fromGreen, int fromBlue, int toRed, int toGreen, int toBlue, int fadeTimeMilliseconds) {
  
  float deltaRed = (float)(fromRed - toRed) / fadeTimeMilliseconds;
  float deltaGreen = (float)(fromGreen - toGreen) / fadeTimeMilliseconds;
  float deltaBlue = (float)(fromBlue - toBlue) / fadeTimeMilliseconds;

  float absRed = (float)fromRed;
  float absGreen = (float)fromGreen;
  float absBlue = (float)fromBlue;
  
  int counter = 0;
  
  while(counter++ < fadeTimeMilliseconds) {
    absRed -= deltaRed;
    absGreen -= deltaGreen;
    absBlue -= deltaBlue;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].red = absRed;
      leds[i].green = absGreen;
      leds[i].blue = absBlue;
    }
    FastLED.show();
    delay(1);
  }
}


