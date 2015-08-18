
#ifndef IRSensor_cpp
#define IRSensor_cpp

#include <stdio.h>
#include "Arduino.h"

class IRSensor {
private:
    int analogInputPin;
    int value;
    bool isTrigged_;
    int trigThreshold;
    int holdTime;
    int remainingHoldTime;
    int randomCubeIndex;
public:
    IRSensor(int analogInputPin, int trigThreshold, int holdTime);
    int getValue();
    bool isTrigged();
    void trig();
    void reduceRemainingHoldTime(int ms);
};

#endif /* IRSensor_cpp */
