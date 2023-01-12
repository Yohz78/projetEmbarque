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
        //BME bme;

    public:

    Handler();

    ~Handler();

    void run();
};
