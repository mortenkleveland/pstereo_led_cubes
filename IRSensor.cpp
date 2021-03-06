
#include "IRSensor.h"

IRSensor::IRSensor(int analogInputPin, int trigThreshold, int holdTime)
{
    this->analogInputPin = analogInputPin;
    this->trigThreshold = trigThreshold;
    this->holdTime = holdTime;
}

int IRSensor::getValue()
{
    int val = analogRead(this->analogInputPin);
    if (val > 1000) { // Most likely error
      return 0;
    }
    return val;
}

int IRSensor::getTrigThreshold()
{
    return this->trigThreshold;
}

bool IRSensor::isTrigged()
{
    return this->isTrigged_;
}

void IRSensor::trig() {
    this->remainingHoldTime = holdTime;
    this->isTrigged_ = true;
}

void IRSensor::reduceRemainingHoldTime(int ms)
{
    this->remainingHoldTime -= ms;
    if (this->remainingHoldTime <= 0) {
        this->remainingHoldTime = 0;
        this->isTrigged_ = false;
    }
}

int IRSensor::getRandomCubeIndex() 
{
    return this->randomCubeIndex;
}

void IRSensor::setRandomCubeIndex(int randomCubeIndex)
{
    this->randomCubeIndex = randomCubeIndex;
}

CRGBPalette16 IRSensor::getPalette()
{
    return this->palette;
}

void IRSensor::setPalette(CRGBPalette16 palette)
{
    this->palette = palette;
}

int IRSensor::getPaletteType()
{
    return this->paletteType;
}

void IRSensor::setPaletteType(int paletteType)
{
    this->paletteType = paletteType;
}

void IRSensor::setHoldTime(int holdTime) 
{
    this->holdTime = holdTime;  
}


