#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "src/handler/handler.h"
//#include "src/bme/bme.h"
int main() {

    std::cout <<"toto" << std::endl;

    bme test(0,0,0);
    //test.harvestDataAndRun();
    Handler handler();
    handler.getHMCvalue();

    return 0;
}
