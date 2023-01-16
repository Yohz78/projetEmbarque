#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <wiringSerial.h>
#include <cstdlib>

#include "src/handler/handler.h"
//#include "src/bme/bme.h"

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
        

        //lecture
        if (serialDataAvail(fd) > 0) { // Vérifie s'il y a des données disponibles pour lecture
            std::string data;
            while (serialDataAvail(fd) > 0) {
                char c = serialGetchar(fd);
                data += c;
            }
            std::cout << "Données reçues : " << data << std::endl;
        }
        else{
            std::cout << "J'ai pas trouvé de données chef !!! " << std::endl;    
        } 
        sleep(1); // Fait une pause pendant 1 seconde  
    }
    serialClose(fd); // Ferme le port série
    return 0;
}