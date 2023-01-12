#include "handler.h"

Handler::Handler(){
    this->hcsr = HCSR50();
    this->hmc = HMC5883L();
}
