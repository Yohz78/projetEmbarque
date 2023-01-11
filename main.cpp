#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "src/handler/handler.h"

int main() {

    std::cout <<"toto" << std::endl;

    bme test(0,0,0);
    test.harvestDataAndRun();

    return 0;
}
