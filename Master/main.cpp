#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>
#include <json/json.h>
#include "src/pca/pca.h"
#include <sstream>
#include <cstring>
//#include <PCA9685.h>

std::string read_sensor_data(int fd) {
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
        return data;
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
        int mvt_tracker = 0;
        std::string string_data = read_sensor_data(fd);
        std::istringstream json_stream(string_data);
        Json::Value root;
        Json::CharReaderBuilder builder;
        JSONCPP_STRING errs;
        bool parsingSuccessful = Json::parseFromStream(builder,
                                                       json_stream,
                                                       &root,
                                                       &errs);
        if (!parsingSuccessful) {
            std::cout << "Error parsing JSON: " << errs << std::endl;
            return 1;
        }
        Json::Value data = root["data"];
        for (unsigned int i = 0; i < data.size(); i++) {
            double presence = data[i]["mvt"].asDouble();
            std::cout << "Presence: " << presence << std::endl;
            if(presence==0 && mvt_tracker==1){
                pca.moveYellowFlag(180);
            }
            if(presence==0 && mvt_tracker==0){
                pca.moveYellowFlag(0);
            }
            if(presence==1){
                pca.moveYellowFlag(90);
                mvt_tracker=1;
            }
        }
    }

    serialClose(fd); // Ferme le port série
    return 0;
}


