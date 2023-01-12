#include "handler.h"

Handler::Handler(){
    this->bme280 = bme();
    this->hmc = HMC5883L();
    this->hcsr = HCSR50();
}
Handler::~Handler()
{

}

void Handler::getHMCvalue(){
    this->hmc.readX();
    this->hmc.readY();
    this->hmc.readZ();
    std::cout<<"x = "<<this->hmc.getX()<<std::endl;
    std::cout<<"y = "<<this->hmc.getY()<<std::endl;
    std::cout<<"z = "<<this->hmc.getZ()<<std::endl;
}
