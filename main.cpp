#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "src/handler/handler.h"

int main() {

    std::cout <<"toto" << std::endl;

    bme test();
    test.harvestDataAndRun();

    return 0;
}
