#include "handler.h"

Handler::Handler(int pin){
    this->hcsr = HCSR50(pin);
    this->hmc = HMC5883L();
}