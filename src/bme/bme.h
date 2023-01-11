#include <iostream>
#include "wiringPi2c.h"
#include "wiringPi.h"
class bme{
    private:

    int fd; // File Descriptor
    float temperature;
    float pression;
    float humidite;

    public:

    bme(); //Ctor
    ~bme(); //Dtor

    void getTemperature();
    void getPression();
    void getHumidite();




}
