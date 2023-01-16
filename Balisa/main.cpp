#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <wiringSerial.h>
#include <cstdlib>

#include "src/handler/handler.h"
//#include "src/bme/bme.h"

/**
 * @brief This function gets a command input from the Master and execute the command. 
 * 
 * @param[in] fd 
 * @param[in] Handler handler 
 */
void loop(int fd,Handler* handler) {
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
            std::cout << handler->getBME().harvestDataAndRun() << std::endl;
            serialPuts(fd,handler->getBME().harvestDataAndRun().c_str());
        }
        if(strcmp(command,"HMC")==0) {
            std::cout << "---------------------HMC---------------------" << std::endl;
            std::cout << "Valeur HMC : " << handler->getHMCvalue() << std::endl;
            serialPuts(fd,handler->getHMCvalue().c_str()); // SENDING DATA
        }
        if(strcmp(command,"HCSR")==0) {
            std::cout << "---------------------HCSR---------------------" << std::endl;
            std::string data = "Mouvement: " + std::to_string(handler->getHCSR().checkMotion()) + "\n";
            serialPuts(fd, data.c_str()); // SENDING DATA
        }
        if(strcmp(command,"all")==0) {
            std::cout << "---------------------BME280---------------------" << std::endl;
            std::cout << handler->getBME().harvestDataAndRun() << std::endl;
            serialPuts(fd,handler->getBME().harvestDataAndRun().c_str());
            std::cout << "---------------------HMC---------------------" << std::endl;
            std::cout << "Valeur HMC : " << handler->getHMCvalue() << std::endl;
            serialPuts(fd,handler->getHMCvalue().c_str()); // SENDING DATA
            std::cout << "---------------------HCSR---------------------" << std::endl;
            std::string data = "Mouvement: " + std::to_string(handler->getHCSR().checkMotion()) + "\n";
            serialPuts(fd, data.c_str()); // SENDING DATA
            std::cout << "----------------------------------------------" << std::endl;
        }
    }
}

int main() {
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyS0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    Handler handler; // Creation du wrapper

    while (true) {
        std::cout << "Valeur de fd=====================> " << fd << std::endl;
        std::cout << "Valeur de serialDataAvail(fd)====> " << serialDataAvail(fd) << std::endl;
        /*int data = rand() % 100; // Génère un nombre aléatoire entre 0 et 99
        std::string data_str = std::to_string(data); // Convertit le nombre en chaîne de caractères
        serialPuts(fd, data_str.c_str()); // Envoie les données sur le port série */ 
        // std::cout << "Données envoyées : " << data_str << std::endl; 

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
                std::cout << handler->getBME().harvestDataAndRun() << std::endl;
                serialPuts(fd,handler->getBME().harvestDataAndRun().c_str());
            }
            if(strcmp(command,"HMC")==0) {
                std::cout << "---------------------HMC---------------------" << std::endl;
                std::cout << "Valeur HMC : " << handler->getHMCvalue() << std::endl;
                serialPuts(fd,handler->getHMCvalue().c_str()); // SENDING DATA
            }
            if(strcmp(command,"HCSR")==0) {
                std::cout << "---------------------HCSR---------------------" << std::endl;
                std::string data = "Mouvement: " + std::to_string(handler->getHCSR().checkMotion()) + "\n";
                serialPuts(fd, data.c_str()); // SENDING DATA
            }
            if(strcmp(command,"all")==0) {
                std::cout << "---------------------BME280---------------------" << std::endl;
                std::cout << handler->getBME().harvestDataAndRun() << std::endl;
                serialPuts(fd,handler->getBME().harvestDataAndRun().c_str());
                std::cout << "---------------------HMC---------------------" << std::endl;
                std::cout << "Valeur HMC : " << handler->getHMCvalue() << std::endl;
                serialPuts(fd,handler->getHMCvalue().c_str()); // SENDING DATA
                std::cout << "---------------------HCSR---------------------" << std::endl;
                std::string data = "Mouvement: " + std::to_string(handler->getHCSR().checkMotion()) + "\n";
                serialPuts(fd, data.c_str()); // SENDING DATA
                std::cout << "----------------------------------------------" << std::endl;
            }
        }
        sleep(5); // Fait une pause pendant 1 seconde
    }

    serialClose(fd); // Ferme le port série
    return 0;
}




/*
int main() {


    Handler handler;
    int fd = serialOpen("/dev/ttyS0", 9600);
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
        //loop(fd,&handler);   

        serialPuts(fd,"TOTO");
        serialPuts(fd,"TOTO1");
        serialPuts(fd,"TOTO2");
        serialPuts(fd,"TOTO3");
        serialPuts(fd,"TOTO4");
        serialPuts(fd,"TOTO5");
        serialPuts(fd,"TOTO6");
        serialPuts(fd,"TOTO7");
        serialPuts(fd,"TOTO8");


    return 0;
}
*/


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
