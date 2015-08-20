
#ifndef IRSensor_cpp
#define IRSensor_cpp

#include <stdio.h>
#include "Arduino.h"
#include "FastLED.h"

class IRSensor {
private:
    int analogInputPin;
    int value;
    bool isTrigged_;
    int trigThreshold;
    int holdTime;
    int remainingHoldTime;
    int randomCubeIndex;
    CRGBPalette16 palette;
    int paletteType; // 0 == palette, 1 == fire2012
public:
    IRSensor(int analogInputPin, int trigThreshold, int holdTime);
    int getValue();
    int getTrigThreshold();
    bool isTrigged();
    void trig();
    void reduceRemainingHoldTime(int ms);
    void setRandomCubeIndex(int randomCubeIndex);
    int getRandomCubeIndex();
    void setPalette(CRGBPalette16 palette);
    int getPaletteType();
    void setPaletteType(int paletteType);
    CRGBPalette16 getPalette();
};

#endif /* IRSensor_cpp */
