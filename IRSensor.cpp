
#include "IRSensor.h"

IRSensor::IRSensor(int analogInputPin, int trigThreshold, int holdTime)
{
    this->analogInputPin = analogInputPin;
    this->trigThreshold = trigThreshold;
    this->holdTime = holdTime;
}

int IRSensor::getValue()
{
    return analogRead(this->analogInputPin);
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
