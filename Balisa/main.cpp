#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <wiringSerial.h>

#include "src/handler/handler.h"
//#include "src/bme/bme.h"
int main() {


    Handler handler;
    int fd = serialOpen("/dev/ttyS0", 9600);
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    while(1){
        loop(fd);
    }
    return 0;
}

void loop(int fd) {
    if (serialDataAvail(fd)) {
        char command[20];
        int index=0;
        while(serialGetchar(fd)!='\n'){
            command[index] = serialGetchar(fd);
            index++;
        }
        command[index]='\0';
        if(strcmp(command,"BME")==0) {
            std::cout << "---------------------BME280---------------------" << std::endl;
            std::cout << handler.getBME().harvestDataAndRun() << std::endl;
            serialPuts(fd,handler.getBME().harvestDataAndRun());
        }
        if(strcmp(command,"HMC")==0) {
            std::cout << "---------------------HMC---------------------" << std::endl;
            std::cout << "Valeur HMC : " << handler.getHMCvalue() << std::endl;
            serialPuts(fd,handler.getHMCvalue()); // SENDING DATA
        }
        if(strcmp(command,"HCSR")==0) {
            std::cout << "---------------------HCSR---------------------" << std::endl;
            std::string data = "Mouvement: " + std::to_string(handler.getHCSR().checkMotion()) + "\n";
            serialPuts(fd, data.c_str()); // SENDING DATA
        }
        if(strcmp(command,"all")==0) {
            std::cout << "---------------------BME280---------------------" << std::endl;
            std::cout << handler.getBME().harvestDataAndRun() << std::endl;
            serialPuts(fd,handler.getBME().harvestDataAndRun());
            std::cout << "---------------------HMC---------------------" << std::endl;
            std::cout << "Valeur HMC : " << handler.getHMCvalue() << std::endl;
            serialPuts(fd,handler.getHMCvalue()); // SENDING DATA
            std::cout << "---------------------HCSR---------------------" << std::endl;
            std::string data = "Mouvement: " + std::to_string(handler.getHCSR().checkMotion()) + "\n";
            serialPuts(fd, data.c_str()); // SENDING DATA
            std::cout << "----------------------------------------------" << std::endl;
        }
    }
}

/*for(int i=0;i<10;i++){
        delay(1);
        std::cout << "-" ;
    }
    std::cout << std::endl;
    std::cout << "---------------------BME280---------------------" << std::endl;
    handler.getBME().harvestDataAndRun();
    std::cout << "---------------------HMC---------------------" << std::endl;
    handler.getHMCvalue();
    std::cout << "---------------------HCSR---------------------" << std::endl;
    std::cout << "Mouvement: " << handler.getHCSR().checkMotion() << std::endl;
    std::cout << "----------------------------------------------" << std::endl;*/
