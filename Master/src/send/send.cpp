#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <typeinfo>

#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <regex>

#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wiringSerial.h>
#include <jsoncpp/json/json.h>

#include "../pca/pca.h"
#include "send.h"

#define INTERVALLE_RECUP 3
#define INTERVALLE_ENVOI_SERVEUR 60

using namespace std;


/**
 * @brief This function read the sensor data from the slave over TX/RX communication
 * 
 * @param int \p fd $File descriptor 
 * @return string 
 */
string read_sensor_data(int fd) {
    PCA9685 pca(1,0x40);
    pca.init();
    while (true) {
        char data[10000];
        int index = 0; 
        while (serialDataAvail(fd) > 0) {
            int c = serialGetchar(fd);
            if (c < 0) {
                cout << "Error: Unable to receive data over UART" << endl;
            }
            data[index] = c;
            index++;
        }
        data[index] = '\0';
        cout << "read_sensor_data: Donnée reçue: "<< data << endl;
        return data;
        // sleep(1); 
    }
}

/**
 * @brief This function handles the yellow flag movements.
 * 
 * @param string_data 
 */
void logicYellowFlag(string string_data, int &mvt_tracker, PCA9685 &pca){
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
    }else{
    double presence = root["HCSR"]["mvt"].asDouble();
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
    }
}

/**
 * @brief This function parse the data from the read_sensor_data(fd) function and move the 
 * Yellow flag accordingly. Then it writes the data into the file data.json
 * 
 * @param int \p fd $File descriptor
 * @param int \p clientSd $Client socket descriptor
 */
void* read_and_write(void* args){
        argsRW* argsFunc = (argsRW*)args;
        int fd = argsFunc->fd;
        int clientSd = argsFunc->clientSd;    
        PCA9685 pca(1,0x40);
        pca.init();
        int mvt_tracker = 0;
        int pos_tracker = 0;
        std::regex pattern("^\\{\"date\":\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\","
                        "\"BME\": \\{\"temperature\": [-+]?[0-9]+.?[0-9]+,"
                        "\"pressure\": [0-9]+.?[0-9]+,"
                        "\"humidity\": [0-9]+.?[0-9]+\\},"
                        "\"HCSR\": \\{\"mvt\": [0-1]\\},"
                        "\"HMC\": \\{\"x\": [-+]?[0-9]+,"
                        "\"y\": [-+]?[0-9]+,"
                        "\"z\":[-+]?[0-9]+\\}\\}$");
        while(true){
            vector<string> dataToSend;
            string string_data = read_sensor_data(fd);
            if(std::regex_match(string_data, pattern)){
                cout << "--------------REGEX MATCH--------------" << endl;
                logicYellowFlag(string_data,mvt_tracker,pca);
                dataToSend.push_back(string_data);
            }
            
            std::cout << "-------------------------------ENVOI----------------------------------" << std::endl;
            cout << "DATATOSEND SIZE: " << dataToSend.size() << endl;
            if(dataToSend.size() == 3){
                cout << "--------------ON A STOCKE 3 ELEMENTS DANS DATATOSEND--------------" << endl;
                pca.moveBlueFlag(90);
                for (auto data : dataToSend){
                        sendData(clientSd,data);
                }
                dataToSend.clear();
            }
            std::cout << "-------------------------------FIN ENVOI----------------------------------" << std::endl;
            
            if(pos_tracker==3){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "read_and_write: Pos tracker :" << pos_tracker << endl;
                cout << "read_and_write: Blue flag to 45°" << endl;
            }else if(pos_tracker==2){
                pca.moveBlueFlag(135);
                pos_tracker=3;
                cout << "read_and_write: Pos tracker :" << pos_tracker << endl;
                cout << "read_and_write: Blue flag to 135°" << endl;
            }else if(pos_tracker==0){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "read_and_write: Pos tracker :" << pos_tracker << endl;
                cout << "read_and_write: Blue flag to 45°" << endl;
            }
            if(string_data.empty()){
            cout << "read_and_write:: No data available, Blue flag back to rest position." << endl;
            pca.moveBlueFlag(180);
            pos_tracker=0;
            } 
            dataToSend.clear();
        sleep(INTERVALLE_RECUP);
            }
        return NULL;
}

/**
 * @brief This function initialize the connection to the server
 * 
 * @return int 
 */
int send_init(){
    string ip = "57.128.34.47";
    const char *serverIp = ip.c_str();
    int port = 1500;
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(serverIp);
    sendSockAddr.sin_port = htons(port);

    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    while(status < 0){
        cerr << "Error connecting to server!" << endl;
        sleep(1);
        status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    }
        
    cout << "Connected to the server!" << endl;
    return clientSd;
}

/**
 * @brief This function sends a string message to the server
 * 
 * @param int \p clientSd $Socket descriptor 
 * @param string \p string_data $Message to be sent 
 */
void sendData(int clientSd, string& string_data) {
    char buffer[4000];
    memset(buffer,0,4000);
    strcpy(buffer,string_data.c_str());
    // std::regex pattern("^\\{\"date\":\"\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\","
    //                     "\"BME\": \\{\"temperature\": [-+]?[0-9]+.?[0-9]+,"
    //                     "\"pressure\": [0-9]+.?[0-9]+,"
    //                     "\"humidity\": [0-9]+.?[0-9]+\\},"
    //                     "\"HCSR\": \\{\"mvt\": [0-1]\\},"
    //                     "\"HMC\": \\{\"x\": [-+]?[0-9]+,"
    //                     "\"y\": [-+]?[0-9]+,"
    //                     "\"z\":[-+]?[0-9]+\\}\\}$");
        // if(std::regex_match(string_data, pattern)){
        // cout << "sendDATA=====> REGEX MATCH =====> SENDING DATA" << endl;
    send(clientSd, &buffer, sizeof(buffer) ,0);
    // }
    // else{
    //     cout << "sendDATA=====> REGEX DID NOT MATCH =====> NOT SENDING DATA" << endl;
    // }
}

/**
 * @brief This function close the connection to the server
 * 
 * @param int \p clientSd $Socket descriptor 
 */
void send_close(int clientSd){
    close(clientSd);
}