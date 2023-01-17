#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
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
            std::ostringstream oss;
            std::cout << "---------------------DATAS---------------------" << std::endl;
            oss << "{";
            oss << handler->getBME().harvestDataAndRun();
            oss << ",";
            oss << handler->getHMCvalue();
            oss << ",";
            oss << handler->getHCSR().checkMotion();
            oss << "}";
            serialPuts(fd,oss.str().c_str());
            std::cout << "Donnees envoyees:" << std::endl;
            std::cout << oss;
            std::cout << std::endl;
            std::cout << "----------------------------------------------" << std::endl;
}


int main() {
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyS0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    Handler handler; // Creation du wrapper

    while (true) {
        
        loop(fd, &handler);
        sleep(1); // Fait une pause pendant 1 seconde  
    }
    serialClose(fd); // Ferme le port série
    return 0;
}

