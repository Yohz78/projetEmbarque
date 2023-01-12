#include "hcsr.h"

HCSR50::HCSR50(){
    this->motionPin = 38;
    pinMode(motionPin, INPUT);
    this->motionDetected = false;
}

bool HCSR50::checkMotion(){
    int value = digitalRead(this->motionPin);
    if (value == HIGH) {
        if (this->motionDetected == false) {
            this->motionDetected = true;
        }
        return true;
    } else {
        this->motionDetected = false;
    }
    return false;
}
