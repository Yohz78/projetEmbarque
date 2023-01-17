#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>
#include "/usr/include/jsoncpp/json/json.h"
#include "src/pca/pca.h"
#include <sstream>
#include <cstring>
#include <curl/curl.h>

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
void read_and_write(int fd){
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




void send(){
    CURL *curl;
    CURLcode res;
    std::string sftpUrl = "sftp://ubuntu@57.128.34.47/Data/data.json";

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, sftpUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, fdopen(open("data.json", O_RDONLY), "rb"));
        curl_easy_setopt(curl, CURLOPT_SSH_PRIVATE_KEYFILE, "opom__227__0_");
        curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PUBLICKEY);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

int main() {

    
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }


    // C  est ici que l'on va avoir besoin des threads, un qui recupere de la data et un qui envoie, les deux a differents intervalles
    read_and_write(fd);

    serialClose(fd); // Ferme le port série
    return 0;
}