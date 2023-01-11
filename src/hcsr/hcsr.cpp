#include "hcsr.h"

HCSR50::HCSR50(int pin){
    motionPin = pin;
    pinMode(motionPin, INPUT);
    motionDetected = false;
}

bool HCSR50::checkMotion(){
    int value = digitalRead(this->motionPin);
    if (value == HIGH) {
        if (this->motionDetected == false) {
            this->motionDetected = true;
            return true;
        }
    } else {
        this->motionDetected = false;
    }
    return false;
}    
