#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>
//#include <json/json.h>
#include "src/pca/pca.h"
//#include <PCA9685.h>

void* read_sensor_data(int fd) {
    PCA9685 pca(1,0x40);
    pca.init();
    while (true) {
        char data[10000];
        int index = 0; 
        while (serialDataAvail(fd) > 0) {
            int c = serialGetchar(fd);
            if (c < 0) {
                std::cout << "Error: Unable to receive data over UART" << std::endl;
            }
            data[index] = c;
            index++;
        }
        data[index] = '\0';
        if (data[index]["mvt"]=="0"){
            pca.moveYellowFlag(0);
        }
        std::cout << "Mouvement : " << data[index]["mvt"] << std::endl;
        sleep(1); // Fait une pause pendant interval secondes
    }
}

int main() {

    
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    PCA9685 pca(1,0x40);
    pca.init();
    
    while (true) {
        read_sensor_data(fd);
    }

    serialClose(fd); // Ferme le port série
    return 0;
}


