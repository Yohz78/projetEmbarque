#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <wiringSerial.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <ctime>

#include "src/handler/handler.h"
//#include "src/bme/bme.h"


/**
 * @brief This function gets a command input from the Master and execute the command. 
 * 
 * @param[in] fd 
 * @param[in] Handler handler 
 */
void loop(int fd,Handler* handler) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss1;
    oss1 << put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    std::string iso_time = oss1.str();
    std::string sep = "\"";
            std::ostringstream oss;
            std::cout << "---------------------DATAS---------------------" << std::endl;
            oss << "{\"timestamp\":" + sep + iso_time + sep;
            oss << ",";
            oss << handler->getBME().harvestDataAndRun();
            oss << ",";
            oss << handler->getHCSR().checkMotion(); 
            oss << ",";
            oss << handler->getHMCvalue();
            oss << "}";
            serialPuts(fd,oss.str().c_str());
            std::cout << "Donnees envoyees:" << std::endl;
            std::cout << oss.str();
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
        sleep(3); // Fait une pause pendant 1 seconde  
    }
    serialClose(fd); // Ferme le port série
    return 0;
}

