#include <iostream>
#include "Adafruit_BME280.h"
#include "wiringPi2c.h"
class bme{
    private:

    int fd; // File Descriptor
    float temperature;
    float pression;
    float humidite;

    public:

    bme(); //Ctor
    ~bme(); //Dtor

    float getTemperature() {return readTemperature();}
    float getPression() {return readPressure();}
    float getHumidite() {return readHumidity();}




}
