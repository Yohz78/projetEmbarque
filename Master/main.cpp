#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>
#include "/usr/include/jsoncpp/json/json.h"
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
        //std::cout << "Donnée reçue"<< data << std::endl;
        return data;
        sleep(1); // Fait une pause pendant interval secondes
    }
}

void read_and_send(int fd){
        PCA9685 pca(1,0x40);
        pca.init();
        while (true) {
        int mvt_tracker = 0;
        std::string string_data = read_sensor_data(fd);
        if(!string_data.empty()){
            std::istringstream json_stream(string_data);
            Json::Value root;
            Json::CharReaderBuilder builder;
            JSONCPP_STRING errs;
            bool parsingSuccessful = Json::parseFromStream(builder,
                                                       json_stream,
                                                       &root,
                                                       &errs);
            if (!parsingSuccessful) {
                std::cout << "Error parsing JSON" << std::endl;
            }
            double presence = root["mvt"].asDouble();
            std::cout << root << std::endl;
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
            std::cout << "Data received and treated"<< std::endl;
        }else{
            std::cout << "No data available, as a result, flags won't move." << std::endl;
        } 
        sleep(3);
        }
}

int main() {

    
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    read_and_send(fd);

    serialClose(fd); // Ferme le port série
    return 0;
}