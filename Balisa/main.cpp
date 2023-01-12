#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "src/handler/handler.h"
//#include "src/bme/bme.h"
int main() {


    Handler handler;
    while(1){
    for(int i=0;i<10;i++){
        sleep(1);
        std::cout << "-" ;
    }
    std::cout << std::endl;
    std::cout << "---------------------BME280---------------------" << std::endl;
    handler.getBME().harvestDataAndRun();
    std::cout << "---------------------HMC---------------------" << std::endl;
    handler.getHMCvalue();
    std::cout << "---------------------HCSR---------------------" << std::endl;
    std::cout << "Mouvement: " << handler.getHCSR().checkMotion() << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    }
    return 0;
}
