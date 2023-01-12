#include "handler.h"

Handler::Handler(){
    this->hcsr = HCSR50(38);
    this->hmc = HMC5883L();
}
