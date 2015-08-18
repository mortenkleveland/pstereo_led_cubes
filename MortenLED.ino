#include "FastLED.h"
#include "IRSensor.h"

#define NUM_LEDS 25
#define NUM_CUBES 21
#define COLOR_ORDER RGB
#define BRIGHTNESS 100
#define FRAMES_PER_SECOND 60
#if defined(__SAM3X8E__)
#define DATA_PIN 6
#else
#define DATA_PIN 6
#endif
#define COOLING 55
#define SPARKING 120

const int SENSOR_PIN = 2;
const int NUM_STATES = 11;
const int DELAY_TIME = 50; // Time in milliseconds
unsigned long timeMs = 0;
unsigned long switchTime = 750;
long cycleTime = switchTime * NUM_CUBES;
long switchCounter = 0;
int sensorState = 0;

// Random light stuff for sequential palettes
int randValues[NUM_CUBES];
int numRandom = 2;
volatile int triggerCounter = 0;

CRGB leds[NUM_LEDS * NUM_CUBES];
CRGBPalette16 currentPalettes[NUM_CUBES];
CRGBPalette16 targetPalettes[NUM_CUBES];
TBlendType currentBlending;

// Colors
const unsigned long red = CRGB::Red;
const unsigned long blue = CRGB::Blue;
const unsigned long aliceBlue = CRGB::AliceBlue;
const unsigned long goldenRod = CRGB::Goldenrod;
const unsigned long gray = CRGB::Gray;
const unsigned long white = CRGB::White;
const unsigned long black = CRGB::Black;

// Palettes
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

extern CRGBPalette16 GreenPalette;
extern const TProgmemPalette16 GreenPalette_p PROGMEM;

extern CRGBPalette16 RedPalette;
extern const TProgmemPalette16 RedPalette_p PROGMEM;

extern CRGBPalette16 AliceBluePalette;
extern const TProgmemPalette16 AliceBluePalette_p PROGMEM;

extern CRGBPalette16 GoldenrodPalette;
extern const TProgmemPalette16 GoldenrodPalette_p PROGMEM;

extern CRGBPalette16 GrayPalette;
extern const TProgmemPalette16 GrayPalette_p PROGMEM;

extern CRGBPalette16 MaroonPalette;
extern const TProgmemPalette16 MaroonPalette_p PROGMEM;

extern CRGBPalette16 WhitePalette;
extern const TProgmemPalette16 WhitePalette_p PROGMEM;

extern CRGBPalette16 BlackPalette;
extern const TProgmemPalette16 BlackPalette_p PROGMEM;

const int NUM_RANDOM_PALETTES = 5;
CRGBPalette16 randomPalettes[NUM_RANDOM_PALETTES] = { AliceBluePalette_p, GoldenrodPalette_p, HeatColors_p, OceanColors_p, GrayPalette_p };

// Infrared sensors
const unsigned int NUM_INFRARED_SENSORS = 2;
const unsigned int INFRARED_SENSOR_HOLD_TIME = 600; // Time in ms
const unsigned int INFRARED_SENSOR_THRESHOLD_LEVEL = 150; // Level between 0 and 1023
IRSensor sensor1(0, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensor2(1, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensors[NUM_INFRARED_SENSORS] = { sensor1, sensor2 };

void setup() 
{
  Serial.begin(9600);
  delay(3000);
  pinMode(SENSOR_PIN, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS * NUM_CUBES);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() 
{  
  // Get serial data from Csound
  /*if (Serial.available()) {
    int brightness = Serial.read();
    FastLED.setBrightness(brightness);
    Serial.println(brightness);
  }
  */

  #if defined(__SAM3X8E__)
  
  #else
  random16_add_entropy(random());
  #endif
  
  /*
  sensorState = digitalRead(SENSOR_PIN);
  attachInterrupt(0, fireSensorChanged, HIGH);
  */
  sensorState = 0;
  static uint8_t startIndex = 0;
  startIndex++;

  int cubeIndex = floor(NUM_CUBES*(timeMs%(NUM_CUBES*switchTime))) / (NUM_CUBES*switchTime);
  //Serial.println(NUM_CUBES*switchTime);
  //Serial.println(NUM_CUBES*(timeMs%(NUM_CUBES*switchTime)));
  //Serial.println(timeMs%(NUM_CUBES*switchTime));
  
  switch(triggerCounter) {
    case 0:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = OceanColors_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = OceanColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = CloudColors_p;
      targetPalettes[8] = HeatColors_p;
      targetPalettes[9] = myRedWhiteBluePalette_p;
      targetPalettes[10] = OceanColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;

      currentBlending = BLEND;
      
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      FastLED.show();
      break;
    case 1:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = CloudColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = CloudColors_p;
      targetPalettes[8] = myRedWhiteBluePalette_p;
      targetPalettes[9] = myRedWhiteBluePalette_p;
      targetPalettes[10] = HeatColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;
      
      currentBlending = NOBLEND;
      
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }

      showSingleCube(cubeIndex, false);
      FastLED.show();
      break;
    case 2:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = CloudColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = HeatColors_p;
      targetPalettes[8] = myRedWhiteBluePalette_p;
      targetPalettes[9] = CloudColors_p;
      targetPalettes[10] = PartyColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;
      
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 3:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = OceanColors_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = OceanColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = CloudColors_p;
      targetPalettes[8] = HeatColors_p;
      targetPalettes[9] = myRedWhiteBluePalette_p;
      targetPalettes[10] = OceanColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;
      
      currentBlending = NOBLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      fire2012(6);
      FastLED.show();
      break;
    case 4:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = CloudColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = CloudColors_p;
      targetPalettes[8] = myRedWhiteBluePalette_p;
      targetPalettes[9] = myRedWhiteBluePalette_p;
      targetPalettes[10] = HeatColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;
      
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      fire2012(10);
      fire2012(15);
      FastLED.show();
      break;
    case 5: // Green
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GreenPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 6:
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      targetPalettes[2] = myRedWhiteBluePalette_p;
      targetPalettes[3] = HeatColors_p;
      targetPalettes[4] = myRedWhiteBluePalette_p;
      targetPalettes[5] = CloudColors_p;
      targetPalettes[6] = PartyColors_p;
      targetPalettes[7] = HeatColors_p;
      targetPalettes[8] = myRedWhiteBluePalette_p;
      targetPalettes[9] = CloudColors_p;
      targetPalettes[10] = PartyColors_p;
      targetPalettes[11] = OceanColors_p;
      targetPalettes[12] = myRedWhiteBluePalette_p;
      targetPalettes[13] = HeatColors_p;
      targetPalettes[14] = myRedWhiteBluePalette_p;
      targetPalettes[15] = OceanColors_p;
      targetPalettes[16] = PartyColors_p;
      targetPalettes[17] = CloudColors_p;
      targetPalettes[18] = HeatColors_p;
      targetPalettes[19] = myRedWhiteBluePalette_p;
      targetPalettes[20] = OceanColors_p;
      
      currentBlending = NOBLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }

      #if defined(__SAM3X8E__)

      #else
      if(timeMs % switchTime == 0) {
        generateNRandomCubes(numRandom, randValues);
      }

      for (int i = 0; i < NUM_CUBES; i++) {
        if (arrayContains(i, randValues)) {
          showInstantly(i);
        } else {
          fillBlack(i);
        }
      }
      #endif
      
      FastLED.show();
      break;
    case 7: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 8: // AliceBlue
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = AliceBluePalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 9: // Goldenrod
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GoldenrodPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 10: // Maroon
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = MaroonPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 11: // Gray
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GrayPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 12: // Gray
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GrayPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    default:
      break;
  }

  uint8_t maxChanges = 20;
  for (int i = 0; i < NUM_CUBES; i++) {
    nblendPaletteTowardPalette(currentPalettes[i], targetPalettes[i], maxChanges);
  }

  // Infrared sensor trigging
  for (int i = 0; i < NUM_INFRARED_SENSORS; i++) {
    // Trig new event if not already trigged and value is over the threshold
    if (!sensors[i].isTrigged() && sensors[i].getValue() > sensors[i].getTrigThreshold()) {
      sensors[i].trig();
      sensors[i].setRandomCubeIndex(random(NUM_CUBES)); // Seed random cube number
      sensors[i].setPalette(getRandomPalette()); // Make the chosen cube light up with a random palette
      Serial.println(sensors[i].getRandomCubeIndex());
    }
    
    // If sensor is trigged, continue countdown
    if (sensors[i].isTrigged()) { 
      sensors[i].reduceRemainingHoldTime(DELAY_TIME);
      currentPalettes[sensors[i].getRandomCubeIndex()] = sensors[i].getPalette();
      targetPalettes[sensors[i].getRandomCubeIndex()] = BlackPalette_p;
      
      //instantWhite(sensors[i].getRandomCubeIndex());
      //instantBlack(sensors[i].getRandomCubeIndex());
      //fire2012(sensors[i].getRandomCubeIndex());
      currentBlending = BLEND;
      FastLED.show();
    }
  }

  timeMs += DELAY_TIME;

  // Timer that changes state
  if(timeMs % 20000 == 0) { // This will fail if timeMs wraps around 2^32 (unsigned)
    triggerCounter++;
    if(triggerCounter > NUM_STATES - 1) {
      triggerCounter = 0;
    }
  }
  
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void fillBlack(int cubeNumber) 
{
  for (int i = 0; i < 16; i++) {
    targetPalettes[cubeNumber][i] = CRGB::Black;
  }
}

void showInstantly(int cubeNumber) 
{
  currentPalettes[cubeNumber] = targetPalettes[cubeNumber];
}

void showSingleCube(int num, bool instantly) 
{
  for (int i = 0; i < NUM_CUBES; i++) {
    if (i == num) {
      if (instantly) {
        showInstantly(i);
      }
    } else {
      fillBlack(i);
    }
  }
}

#if defined(__SAM3X8E__)
#else

void generateNRandomCubes(int n, int *values)
{
  // Boundary check
  if (n > NUM_CUBES) {
    n = NUM_CUBES;
  }

  // Add n random numbers to array
  for (int i = 0; i < n; i++) {
    int newRand = random(NUM_CUBES);
    while (arrayContains(newRand, values)) {
      newRand = random(NUM_CUBES);
    }
    values[i] = newRand;
  }
}

#endif

bool arrayContains(int element, int arr[]) {
  int arraySize = sizeof(arr)/sizeof(int);
  for (int i = 0; i < arraySize; i++) {
    if (element == arr[i]) {
      return true;
    }
  }
  return false;
}

CRGBPalette16 getRandomPalette() 
{
  return randomPalettes[random(NUM_RANDOM_PALETTES)];
}

void fire2012(int cubeNumber) 
{
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

void instantWhite(int cubeNumber) 
{
  for(int j = 0; j < NUM_LEDS; j++) {
      leds[j + cubeNumber * NUM_LEDS] = HeatColor(255);
  }
}


void instantBlack(int cubeNumber) 
{
  for(int j = 0; j < NUM_LEDS; j++) {
      leds[j + cubeNumber * NUM_LEDS] = HeatColor(0);
  }
}

int prevSensorChangedTime = 0;
 
void fireSensorChanged() 
{
  int currentTime = millis();
  int interval = 200; // Do not want to trigger new events inside this time interval
  if(currentTime - prevSensorChangedTime > interval) {
    prevSensorChangedTime = currentTime;
    triggerCounter++;
    if (triggerCounter >= NUM_STATES) {
      triggerCounter = 0;
    }
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, CRGBPalette16 palette) 
{
  uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, colorIndex, brightness, currentBlending);
    colorIndex += 1;
  }
}


void FillLEDsFromPaletteColors(uint8_t colorIndex, CRGBPalette16 palette, int cubeNumber) 
{
  uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i + NUM_LEDS * cubeNumber] = ColorFromPalette(palette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void ChangePalettePeriodically(CRGBPalette16 palette) 
{
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
void SetupTotallyRandomPalette(CRGBPalette16 palette) 
{
  for (int i = 0; i < 16; i++) {
    palette[i] = CHSV(random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette(CRGBPalette16 palette) 
{
  // 'black out' all 16 palette entries...
  fill_solid(palette, 16, CRGB::Black);
  // and set every fourth one to white.
  palette[0] = CRGB::White;
  palette[4] = CRGB::White;
  palette[8] = CRGB::White;
  palette[12] = CRGB::White;

}

void SetupBlackPalette(CRGBPalette16 palette) 
{
  fill_solid(palette, 16, CRGB::Black);
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette(CRGBPalette16 palette) 
{
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

const TProgmemPalette16 GreenPalette_p PROGMEM =
{
  CRGB::Green,
  CRGB::Gray,
  CRGB::Green,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,

  CRGB::Green,
  CRGB::Gray,
  CRGB::Green,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
};

const TProgmemPalette16 RedPalette_p PROGMEM =
{
  red, red, red, red,
  red, red, red, red,
  red, red, red, red,
  red, red, red, red
};

const TProgmemPalette16 AliceBluePalette_p PROGMEM =
{
  aliceBlue, aliceBlue, aliceBlue, aliceBlue,
  aliceBlue, aliceBlue, aliceBlue, aliceBlue,
  aliceBlue, aliceBlue, aliceBlue, aliceBlue,
  aliceBlue, aliceBlue, aliceBlue, aliceBlue
};


const TProgmemPalette16 GoldenrodPalette_p PROGMEM =
{
  aliceBlue, goldenRod, goldenRod, goldenRod,
  goldenRod, goldenRod, goldenRod, goldenRod,
  gray, goldenRod, goldenRod, goldenRod,
  goldenRod, goldenRod, goldenRod, goldenRod
};


const TProgmemPalette16 GrayPalette_p PROGMEM =
{
  gray, gray, gray, gray,
  gray, gray, gray, gray,
  gray, gray, gray, gray,
  gray, gray, gray, gray
};

const TProgmemPalette16 MaroonPalette_p PROGMEM =
{
  CRGB::Green,
  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,

  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,

  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,

  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,
  CRGB::Maroon,
};

const TProgmemPalette16 WhitePalette_p PROGMEM =
{
  white, white, white, white,
  white, white, white, white,
  white, white, white, white,
  white, white, white, white
};

const TProgmemPalette16 BlackPalette_p PROGMEM =
{
  black, white, black, white,
  black, white, black, white,
  black, white, black, white,
  black, white, black, white
};

