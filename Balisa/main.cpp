#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "src/handler/handler.h"
//#include "src/bme/bme.h"
int main() {

    Handler handler;
    std::cout << "---------------------BME280---------------------" << std::endl;
    handler.getBME().harvestDataAndRun();
    std::cout << "---------------------HMC---------------------" << std::endl;
    handler.getHMCvalue();
    std::cout << "---------------------HCSR---------------------" << std::endl;
    std::cout << "Mouvement: " << handler.getHCSR().checkMotion() << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    return 0;
}
