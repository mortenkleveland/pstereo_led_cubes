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
const int NUM_STATES = 5;
int sensorState = 0;
int prevSensorState = sensorState;
volatile int triggerCounter = 0;

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;

TBlendType currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() { 
  Serial.begin(9600);
  delay(1000);
  pinMode(SENSOR_PIN, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowColors_p;
  targetPalette = currentPalette;
  currentBlending = BLEND;
}

void loop() {
  random16_add_entropy(random());
  sensorState = digitalRead(SENSOR_PIN);
  int potmeterValue = analogRead(ANALOG_IN)/4;
  FastLED.setBrightness(potmeterValue);
  attachInterrupt(0, fireSensorChanged, HIGH);

  switch(triggerCounter) {
    case 0:
      setColor(0, 100, 0);
      FastLED.show();
      delay(1000);
      fadeTo(100, 0, 0, 500);
      setColor(100, 0, 0);
      FastLED.show();
      delay(1000);
      fadeTo(0, 100, 0, 500);
      break;
    case 1:
      fire2012();
      FastLED.show();
      break;
    case 2:
      ChangePalettePeriodically();
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; /* motion speed */
      FillLEDsFromPaletteColors(startIndex);
      FastLED.show();
      break;
    case 3:
      SetupBlackAndWhiteStripedPalette();
      currentBlending = BLEND;
      FillLEDsFromPaletteColors(startIndex++);
      FastLED.show();
      break;
    case 4:
      targetPalette = CloudColors_p; 
      currentBlending = BLEND;
      FillLEDsFromPaletteColors(startIndex++);
      FastLED.show();
      break;
    default:
      break;
  }
   
  uint8_t maxChanges = 24;
  nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  prevSensorState = sensorState;
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

void fadeTo(int toRed, int toGreen, int toBlue, int fadeTimeMilliseconds) {
  CRGB tempLeds[NUM_LEDS]; 
  memcpy(tempLeds, leds, NUM_LEDS * sizeof(CRGB)); // Store current RGB values for all LEDs

  float deltas[NUM_LEDS][3]; // Each LED can have different source/dest values
  for (int i = 0; i < NUM_LEDS; i++) {
    deltas[i][0] = (float)(tempLeds[i].red - toRed) / fadeTimeMilliseconds;
    deltas[i][1] = (float)(tempLeds[i].green - toGreen) / fadeTimeMilliseconds;
    deltas[i][2] = (float)(tempLeds[i].blue - toBlue) / fadeTimeMilliseconds;
  }

  float absoluteValues[NUM_LEDS][3]; // Must store absoulte color values in a separate array
  for (int i = 0; i < NUM_LEDS; i++) {
    absoluteValues[i][0] = (float)tempLeds[i].red;
    absoluteValues[i][1] = (float)tempLeds[i].green;
    absoluteValues[i][2] = (float)tempLeds[i].blue;
  }
  
  int counter = 0;
  
  while(counter++ < fadeTimeMilliseconds) {

    for (int i = 0; i < NUM_LEDS; i++) {
      absoluteValues[i][0] -= deltas[i][0];
      absoluteValues[i][1] -= deltas[i][1];
      absoluteValues[i][2] -= deltas[i][2];
    
      leds[i].red = absoluteValues[i][0];
      leds[i].green = absoluteValues[i][1];
      leds[i].blue = absoluteValues[i][2];
    }
    FastLED.show();
    delay(1);
  }
}

int prevSensorChangedTime = 0;
 
void fireSensorChanged() {
  int currentTime = millis();
  int interval = 200; // Do not want to trigger new events inside this time interval
  if(currentTime - prevSensorChangedTime > interval) {
    prevSensorChangedTime = currentTime;
    triggerCounter++;
    if (triggerCounter >= NUM_STATES) {
      triggerCounter = 0;
    }
    Serial.println(triggerCounter);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
    if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette() {
  for (int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV(random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid(targetPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  targetPalette[0] = CRGB::White;
  targetPalette[4] = CRGB::White;
  targetPalette[8] = CRGB::White;
  targetPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green  = CHSV(HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};


