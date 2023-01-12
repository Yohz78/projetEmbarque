#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "../hcsr/hcsr.h"
#include "../bme/bme.h"
#include "../hmc/hmc.h"

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

    void getHMCvalue();

    //void run();
};
