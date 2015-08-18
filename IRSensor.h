
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
public:
    IRSensor(int analogInputPin, int trigThreshold, int holdTime);
    int getValue();
    int getTrigThreshold();
    bool isTrigged();
    void trig();
    void reduceRemainingHoldTime(int ms);
    int getRandomCubeIndex();
    void setRandomCubeIndex(int randomCubeIndex);
    void setPalette(CRGBPalette16 palette);
    CRGBPalette16 getPalette();
};

#endif /* IRSensor_cpp */
