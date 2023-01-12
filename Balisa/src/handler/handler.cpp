#include "handler.h"

Handler::Handler(){
    //this->hcsr = HCSR50();
    this->hmc = HMC5883L();
    this->bme280 = bme();
}

Handler::getHMCvalue(){
    std::cout<<"x = "<<this->hmc.getX()<<std::endl;
    std::cout<<"y = "<<this->hmc.getY()<<std::endl;
    std::cout<<"z = "<<this->hmc.getZ()<<std::endl;
}
