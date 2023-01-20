#include "hcsr.h"


/**
 * @brief Construct a new HCSR50::HCSR50 object
 * 
 */
HCSR50::HCSR50(){
    this->motionPin = 28;
    wiringPiSetup();
    pinMode(this->motionPin, INPUT);
    this->motionDetected = false;
}

/**
 * @brief Destroy the HCSR50::HCSR50 object
 * 
 */
HCSR50::~HCSR50()
{

}

/**
 * @brief Method to check if the sensor detected motion in its detection radius.
 * 
 * @return true 
 * @return false 
 */
std::string HCSR50::checkMotion(){
    int value = digitalRead(this->motionPin);
    if (value == HIGH) {
        if (this->motionDetected == false) {
            this->motionDetected = true;
        }
        return "\"HCSR\": {\"mvt\": 1}";
    } else {
        this->motionDetected = false;
        return "\"HCSR\": {\"mvt\": 0}";
    } 
}

std::string HCSR50::watcherMotion(){
    int value = digitalRead(this->motionPin);
    if (value == HIGH) {
        if (this->motionDetected == false) {
            this->motionDetected = true;
        }
        return "1";
    } else {
        this->motionDetected = false;
        return "0";
    } 
}