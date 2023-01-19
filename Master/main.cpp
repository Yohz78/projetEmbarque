#include <wiringSerial.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <pthread.h>
#include <jsoncpp/json/json.h>

#include "src/pca/pca.h"
#include "send.cpp"

#define INTERVALLE_RECUP 5
#define INTERVALLE_ENVOI_SERVEUR 60

using namespace boost::asio;

/**
 * @brief This function read the sensor data from the slave over TX/RX communication
 * 
 * @param fd 
 * @return std::string 
 */
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

/**
 * @brief This function parse the data from the read_sensor_data(fd) function and move the 
 * Yellow flag accordingly. Then it writes the data into the file data.json
 * 
 * @param fd 
 */
void read_and_write(int fd, std::vector<Json::Value> &res){
        PCA9685 pca(1,0x40);
        pca.init();
        int mvt_tracker = 0;
        int i = 0;
        int pos_tracker = 0;

        while (true) {
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
            if(pos_tracker==3){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                std::cout << "Blue flag to 45°" << std::endl;
            }else if(pos_tracker==2){
                pca.moveBlueFlag(135);
                pos_tracker=3;
                std::cout << "Blue flag to 135°" << std::endl;
            }else if(pos_tracker==0){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                std::cout << "Blue flag to 45°" << std::endl;
            }
            //Yellow flag logic
            double presence = root["mvt"].asDouble();
            if(presence==0 && mvt_tracker==1){
                pca.moveYellowFlag(180);
                std::cout << "Yellow flag to 180°" << std::endl;
            }
            if(presence==0 && mvt_tracker==0){
                pca.moveYellowFlag(0);
                std::cout << "Yellow flag to 0°" << std::endl;
            }    
            if(presence==1){
                pca.moveYellowFlag(90);
                std::cout << "Yellow flag to 90°" << std::endl;
                mvt_tracker=1;
            }    
            res.push_back(root);
            i++;
            if(i > 5){
                break;
            }
        }else{
            std::cout << "No data available, Blue flag back to rest position." << std::endl;
            pca.moveBlueFlag(180);
            pos_tracker=0;
        } 
        sleep(INTERVALLE_RECUP);
        }
        // pthread_exit(NULL);
}


int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    vector<Json::Value> res;
    read_and_write(fd,res);

    for(auto data: res){
        std::cout << data;
    }

    std::cout << "-------------------------------ENVOI----------------------------------" << std::endl;
    sendData(res);
    res.clear();
    std::cout << "-------------------------------FIN ENVOI----------------------------------" << std::endl;
    
    /*pthread_t thread_rw, thread_send;

    pthread_create(&thread_rw, NULL, read_and_write, &fd, &res);
    pthread_create(&thread_send, NULL,send_boost, &fd, &res);
    pthread_join(thread_rw, NULL);
    pthread_join(thread_send, NULL);*/ 

    // C  est ici que l'on va avoir besoin des threads, un qui recupere de la data et un qui envoie, les deux a differents intervalles
    //read_and_write(fd, res);

    serialClose(fd); // Ferme le port série
    return 0;
}