#ifndef HANDLER_H
#define HANDLER_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "../hcsr/hcsr.h"
#include "../bme/bme.h"
#include "../hmc/hmc.h"

/**
 * @class Handler
 * @brief This class is a wrapper for the 3 sensors, the BME 280, the HCSR50 and the HMC5883L
 */
class Handler{
    private:
        HCSR50 hcsr;
        HMC5883L hmc;
        bme bme280;

    public:

    Handler();

    ~Handler();

    HCSR50 getHCSR() {return hcsr;}
    HMC5883L getHMC(){return hmc;}
    bme getBME(){return bme280;}

    std::string getHMCvalue();

    //void run();
};


#endif //SEND_H