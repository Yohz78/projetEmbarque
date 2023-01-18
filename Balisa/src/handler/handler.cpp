#include "handler.h"


/**
 * @brief Construct a new Handler:: Handler object
 * 
 */
Handler::Handler(){
    this->bme280 = bme();
    this->hmc = HMC5883L();
    this->hcsr = HCSR50();
}

/**
 * @brief Destroy the Handler:: Handler object
 * 
 */
Handler::~Handler()
{

}
/**
 * @brief This function return a string with the x,y and z value of the HMC sensor.
 * 
 * @return std::string 
 */
std::string Handler::getHMCvalue(){
    this->hmc.readX();
    this->hmc.readY();
    this->hmc.readZ();
    std::string flux = "\"HMC\": {\"x\": " + std::to_string(this->hmc.getX())+
                       ",\"y\": " + std::to_string(this->hmc.getY())+
                       ",\"z\" :" + std::to_string(this->hmc.getZ())+"}";
    return flux;
}
