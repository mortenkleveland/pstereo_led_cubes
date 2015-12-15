#include "FastLED.h"
#include "IRSensor.h"

#define NUM_LEDS 25
#define NUM_CUBES 21
#define COLOR_ORDER RGB
#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 60
#if defined(__SAM3X8E__)
#define DATA_PIN 6
#else
#define DATA_PIN 6
#endif
#define COOLING 55
#define SPARKING 120

const int SENSOR_PIN = 2;
const int NUM_STATES = 17;
const int DELAY_TIME = 50; // Time in milliseconds
unsigned long timeMs = 0;
unsigned long switchTime = 750;
long cycleTime = switchTime * NUM_CUBES;
long switchCounter = 0;
int sensorState = 0;

// Random light stuff for sequential palettes
int randValues[NUM_CUBES];
int numRandom = 2;
volatile int triggerCounter = 1;

CRGB leds[NUM_LEDS * NUM_CUBES];
CRGBPalette16 currentPalettes[NUM_CUBES];
CRGBPalette16 targetPalettes[NUM_CUBES];
TBlendType currentBlending;


// Infrared sensors
const unsigned int NUM_INFRARED_SENSORS = 0;
const unsigned int INFRARED_SENSOR_HOLD_TIME = 600; // Time in ms
const unsigned int INFRARED_SENSOR_THRESHOLD_LEVEL = 250; // Level between 0 and 1023
IRSensor sensor1(0, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensor2(1, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensor3(2, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensor4(3, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensor5(3, INFRARED_SENSOR_THRESHOLD_LEVEL, INFRARED_SENSOR_HOLD_TIME);
IRSensor sensors[NUM_INFRARED_SENSORS] = { };//, sensor2, sensor3, sensor4, sensor5 };


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

extern CRGBPalette16 BluePalette;
extern const TProgmemPalette16 BluePalette_p PROGMEM;
 
extern CRGBPalette16 AliceBluePalette;
extern const TProgmemPalette16 AliceBluePalette_p PROGMEM;

extern CRGBPalette16 GoldenrodPalette;
extern const TProgmemPalette16 GoldenrodPalette_p PROGMEM;

extern CRGBPalette16 GrayPalette;
extern const TProgmemPalette16 GrayPalette_p PROGMEM;

extern CRGBPalette16 MaroonPalette;
extern const TProgmemPalette16 MaroonPalette_p PROGMEM;

extern CRGBPalette16 ChristmasPalette;
extern const TProgmemPalette16 ChristmasPalette_p PROGMEM;

extern CRGBPalette16 WhitePalette;
extern const TProgmemPalette16 WhitePalette_p PROGMEM;

extern CRGBPalette16 BlackPalette;
extern const TProgmemPalette16 BlackPalette_p PROGMEM;

const int NUM_RANDOM_PALETTES = 5;
CRGBPalette16 randomPalettes[NUM_RANDOM_PALETTES] = { AliceBluePalette_p, GoldenrodPalette_p, HeatColors_p, OceanColors_p, GrayPalette_p };


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
  #if defined(__SAM3X8E__)
  #else
  random16_add_entropy(random());
  #endif

  sensorState = 0;
  static uint8_t startIndex = 0;
  startIndex++;
  int cubeIndex = floor(NUM_CUBES*(timeMs%(NUM_CUBES*switchTime))) / (NUM_CUBES*switchTime);

  //Serial.println(triggerCounter);
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
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = BluePalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
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
            
      currentBlending = BLEND;
      for (int i = 0; i < NUM_CUBES; i++) {
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      FastLED.show();
      break;
      
      FastLED.show();
      break;
    case 3: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 4:
      Serial.println("AliceBlue");
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = AliceBluePalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 5:
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
      FastLED.show();
      break;
    case 6: // Green
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GreenPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      targetPalettes[0] = RedPalette_p;
      targetPalettes[2] = RedPalette_p;
      targetPalettes[5] = RedPalette_p;
      targetPalettes[9] = RedPalette_p;
      targetPalettes[15] = RedPalette_p;
      targetPalettes[17] = RedPalette_p;
      targetPalettes[19] = RedPalette_p;
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 7:
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
      FastLED.show();
      break;
    case 8: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 9: // Oceancolors
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = OceanColors_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 10: // Gray
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = GrayPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 11: 
      targetPalettes[0] = CloudColors_p;
      targetPalettes[1] = myRedWhiteBluePalette_p;
      targetPalettes[2] = PartyColors_p;
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
    case 12: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 13: // White
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = WhitePalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 14: // Heatcolors
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = HeatColors_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 15: // Blue
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = BluePalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      fire2012(1);
      currentBlending = BLEND;
      FastLED.show();
      break;
    case 16: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
    default: // Red
      for (int i = 0; i < NUM_CUBES; i++) {
        targetPalettes[i] = RedPalette_p;
        FillLEDsFromPaletteColors(startIndex, currentPalettes[i], i);
      }
      currentBlending = BLEND;
      FastLED.show();
      break;
  } 

  uint8_t maxChanges = 25;
  for (int i = 0; i < NUM_CUBES; i++) {
    nblendPaletteTowardPalette(currentPalettes[i], targetPalettes[i], maxChanges);
  }

  // Infrared sensor trigging
  for (int i = 0; i < NUM_INFRARED_SENSORS; i++) {
    Serial.println(sensors[0].getValue());
    // Trig new event if not already trigged and value is over the threshold
    if (!sensors[i].isTrigged() && sensors[i].getValue() > sensors[i].getTrigThreshold()) {
      /*Serial.println(i);
      Serial.println(sensors[i].getValue() > sensors[i].getTrigThreshold());
      Serial.println(sensors[i].getValue());
      Serial.println("\n");*/
      int holdTime = getRandomTime(50, 800);
      Serial.println(holdTime);
      sensors[i].setHoldTime(holdTime);
      sensors[i].trig();
      sensors[i].setRandomCubeIndex(random(NUM_CUBES)); // Seed random cube number

      // Small hack here, since fire2012() doesn't use a palette
      if (random(4) == 0) { // 25% chance of fire2012
        sensors[i].setPaletteType(1);
      } else { // if not, use palette
        sensors[i].setPaletteType(0);
        sensors[i].setPalette(getRandomPalette()); // Make the chosen cube light up with a random palette
      }
      //Serial.println(sensors[i].getRandomCubeIndex());
    }
    
    // If sensor is trigged, continue countdown
    if (sensors[i].isTrigged()) {
      int index = sensors[i].getRandomCubeIndex();
      int oppositeIndex = (sensors[i].getRandomCubeIndex() + (int(NUM_CUBES/2))) % NUM_CUBES; // Also change cube on "opposite" side
      if (sensors[i].getPaletteType() == 0) { // Ordinary palette
        currentPalettes[index] = sensors[i].getPalette();
        currentPalettes[oppositeIndex] = sensors[i].getPalette();
        targetPalettes[index] = BlackPalette_p;
        targetPalettes[oppositeIndex] = BlackPalette_p;
      } else if (sensors[i].getPaletteType() == 1) { // fire2012
        instantBlack(index);
        instantBlack(oppositeIndex);
        fire2012(index);
        fire2012(oppositeIndex);
      }
      sensors[i].reduceRemainingHoldTime(DELAY_TIME);
      currentBlending = BLEND;
      FastLED.show();
    }
  }

  timeMs += DELAY_TIME;

  // Timer that changes state
  if(timeMs % 100000 == 0) { // This will fail if timeMs wraps around 2^32 (unsigned)
    triggerCounter++;
    if(triggerCounter > NUM_STATES - 1) {
      triggerCounter = 0;
    }
  }
  if(timeMs > 1073741824) { // 2^30
    timeMs = 0;
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
  //uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += 1;
  }
}


void FillLEDsFromPaletteColors(uint8_t colorIndex, CRGBPalette16 palette, int cubeNumber) 
{
  //uint8_t brightness = 255;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i + NUM_LEDS * cubeNumber] = ColorFromPalette(palette, colorIndex, BRIGHTNESS, currentBlending);
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
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::Green,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
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

const TProgmemPalette16 BluePalette_p PROGMEM =
{
  blue, blue, blue, blue,
  blue, blue, blue, blue,
  blue, blue, blue, blue,
  blue, blue, blue, blue
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

const TProgmemPalette16 ChristmasPalette_p PROGMEM =
{
  red, red, red, CRGB::Green,
  red, CRGB::Green, CRGB::Green, red,
  red, CRGB::Green, red, red,
  CRGB::Green, red, red, CRGB::Green
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
  black, white, white, white,
  white, white, white, white
};

const TProgmemPalette16 BlackPalette_p PROGMEM =
{
  black, white, black, white,
  black, white, black, white,
  black, white, black, white,
  black, white, black, white
};

int getRandomTime(int lowerBound, int upperBound) 
{
  return (random(upperBound - lowerBound) + lowerBound);
}

