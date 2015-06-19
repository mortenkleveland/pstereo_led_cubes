#include "FastLED.h"

#define NUM_LEDS 25
#define NUM_CUBES 2
#define COLOR_ORDER RGB
#define BRIGHTNESS 255
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

CRGB leds[NUM_LEDS * NUM_CUBES];
//CRGB cubes[NUM_CUBES][NUM_LEDS];
CRGBPalette16 currentPalettes[NUM_CUBES];
CRGBPalette16 targetPalettes[NUM_CUBES];

TBlendType currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() { 
  delay(3000);
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS * NUM_CUBES);
  //FastLED.addLeds<WS2811, DATA_PIN, RGB>(cubes[1], NUM_LEDS * NUM_CUBES);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalettes[0] = RainbowColors_p;
  targetPalettes[0] = currentPalettes[0];

  currentPalettes[1] = CloudColors_p;
  targetPalettes[1] = currentPalettes[1];
  
  currentBlending = BLEND;
}

void loop() {
  random16_add_entropy(random());
  sensorState = digitalRead(SENSOR_PIN);
  int potmeterValue = analogRead(ANALOG_IN)/4;
  //FastLED.setBrightness(potmeterValue);
  attachInterrupt(0, fireSensorChanged, HIGH);
  static uint8_t startIndex = 0;
  startIndex++;
  
  switch(triggerCounter) {
    case 0:
      targetPalettes[1] = OceanColors_p;
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
        FastLED.show();
      }
      fire2012(0);            
      FastLED.show();
      break;
    case 1:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }

      if (startIndex % random8(60, 70) > random8(25, 37)) {
        fillBlack(1);
        showInstantly(0);
      } else {
        fillBlack(0);
        showInstantly(1);
      }
      FastLED.show();
      break;
    case 2:
      targetPalettes[0] = myRedWhiteBluePalette_p;
      targetPalettes[1] = CloudColors_p;
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      FastLED.show();
      break;
    case 3:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = HeatColors_p;
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      FastLED.show();
      break;
    case 4:
      targetPalettes[0] = myRedWhiteBluePalette_p;
      targetPalettes[1] = CloudColors_p;
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      FastLED.show();
      break;
    default:
      break;
  }

  uint8_t maxChanges = 20;
  for (int i = 0; i < NUM_CUBES; i++) {
    nblendPaletteTowardPalette(currentPalettes[i], targetPalettes[i], maxChanges);
  }

  //nblendPaletteTowardPalette(currentPalettes[0], targetPalettes[0], maxChanges);
  prevSensorState = sensorState;
  FastLED.delay(1000 / (potmeterValue + 1));
}

void fillBlack(int cubeNumber) {
  for (int i = 0; i < 16; i++) {
    currentPalettes[cubeNumber][i] = CRGB::Black;
  }
}

void showInstantly(int cubeNumber) {
  currentPalettes[cubeNumber] = targetPalettes[cubeNumber];
}

//void setColor(int red, int green, int blue) {
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i].setRGB(red, green, blue);
//  }
//  FastLED.show();
//}
//
//void fadeTo(int toRed, int toGreen, int toBlue, int fadeTimeMilliseconds) {
//  CRGB tempLeds[NUM_LEDS]; 
//  memcpy(tempLeds, leds, NUM_LEDS * sizeof(CRGB)); // Store current RGB values for all LEDs
//
//  float deltas[NUM_LEDS][3]; // Each LED can have different source/dest values
//  for (int i = 0; i < NUM_LEDS; i++) {
//    deltas[i][0] = (float)(tempLeds[i].red - toRed) / fadeTimeMilliseconds;
//    deltas[i][1] = (float)(tempLeds[i].green - toGreen) / fadeTimeMilliseconds;
//    deltas[i][2] = (float)(tempLeds[i].blue - toBlue) / fadeTimeMilliseconds;
//  }
//
//  float absoluteValues[NUM_LEDS][3]; // Must store absoulte color values in a separate array
//  for (int i = 0; i < NUM_LEDS; i++) {
//    absoluteValues[i][0] = (float)tempLeds[i].red;
//    absoluteValues[i][1] = (float)tempLeds[i].green;
//    absoluteValues[i][2] = (float)tempLeds[i].blue;
//  }
//  
//  int counter = 0;
//  
//  while(counter++ < fadeTimeMilliseconds) {
//
//    for (int i = 0; i < NUM_LEDS; i++) {
//      absoluteValues[i][0] -= deltas[i][0];
//      absoluteValues[i][1] -= deltas[i][1];
//      absoluteValues[i][2] -= deltas[i][2];
//    
//      leds[i].red = absoluteValues[i][0];
//      leds[i].green = absoluteValues[i][1];
//      leds[i].blue = absoluteValues[i][2];
//    }
//    FastLED.show();
//    delay(1);
//  }
//}


void fire2012(int cubeNumber) {
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
      leds[j + cubeNumber * NUM_LEDS] = HeatColor(heat[j]);
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

void FillLEDsFromPaletteColors(uint8_t colorIndex, CRGBPalette16 palette) {
  uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, colorIndex, brightness, currentBlending);
    colorIndex += 1;
  }
}


void FillLEDsFromPaletteColors(uint8_t colorIndex, CRGBPalette16 palette, int cubeNumber) {
  uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i + NUM_LEDS * cubeNumber] = ColorFromPalette(palette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically(CRGBPalette16 palette) {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { palette = RainbowColors_p;         currentBlending = BLEND; }
    if( secondHand == 10)  { palette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 15)  { palette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette(palette);             currentBlending = BLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette(palette);              currentBlending = BLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette(palette);       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette(palette);       currentBlending = BLEND; }
    if( secondHand == 40)  { palette = CloudColors_p;           currentBlending = BLEND; }
    if( secondHand == 45)  { palette = PartyColors_p;           currentBlending = BLEND; }
    if( secondHand == 50)  { palette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 55)  { palette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette(CRGBPalette16 palette) {
  for (int i = 0; i < 16; i++) {
    palette[i] = CHSV(random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette(CRGBPalette16 palette) {
  // 'black out' all 16 palette entries...
  fill_solid(palette, 16, CRGB::Black);
  // and set every fourth one to white.
  palette[0] = CRGB::White;
  palette[4] = CRGB::White;
  palette[8] = CRGB::White;
  palette[12] = CRGB::White;

}

void SetupBlackPalette(CRGBPalette16 palette) {
  fill_solid(palette, 16, CRGB::Black);
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette(CRGBPalette16 palette) {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green  = CHSV(HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  palette = CRGBPalette16( 
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


