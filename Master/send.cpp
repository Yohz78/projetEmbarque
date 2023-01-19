#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <sstream>
#include <cstring>
#include <string>
#include <vector>

#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wiringSerial.h>
#include <jsoncpp/json/json.h>

#include "src/pca/pca.h"

#define INTERVALLE_RECUP 5
#define INTERVALLE_ENVOI_SERVEUR 60

using namespace std;

/**
 * @brief This function read the sensor data from the slave over TX/RX communication
 * 
 * @param fd 
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
        //cout << "Donnée reçue"<< data << endl;
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
void read_and_write(int fd, vector<Json::Value> &res){
        PCA9685 pca(1,0x40);
        pca.init();
        int mvt_tracker = 0;
        int i = 0;
        int pos_tracker = 0;

        while (true) {
        string string_data = read_sensor_data(fd);
        if(!string_data.empty()){
            istringstream json_stream(string_data);
            Json::Value root;
            Json::CharReaderBuilder builder;
            JSONCPP_STRING errs;
            bool parsingSuccessful = Json::parseFromStream(builder,
                                                       json_stream,
                                                       &root,
                                                       &errs);
            if (!parsingSuccessful) {
                cout << "Error parsing JSON" << endl;
            }
            if(pos_tracker==3){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "Blue flag to 45°" << endl;
            }else if(pos_tracker==2){
                pca.moveBlueFlag(135);
                pos_tracker=3;
                cout << "Blue flag to 135°" << endl;
            }else if(pos_tracker==0){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "Blue flag to 45°" << endl;
            }
            //Yellow flag logic
            double presence = root["mvt"].asDouble();
            if(presence==0 && mvt_tracker==1){
                pca.moveYellowFlag(180);
                cout << "Yellow flag to 180°" << endl;
            }
            if(presence==0 && mvt_tracker==0){
                pca.moveYellowFlag(0);
                cout << "Yellow flag to 0°" << endl;
            }    
            if(presence==1){
                pca.moveYellowFlag(90);
                cout << "Yellow flag to 90°" << endl;
                mvt_tracker=1;
            }    
            cout << root << endl;
            res.push_back(root);
            i++;
            if(i > 5){
                break;
            }
        }else{
            cout << "No data available, Blue flag back to rest position." << endl;
            pca.moveBlueFlag(180);
            pos_tracker=0;
        } 
        sleep(INTERVALLE_RECUP);
        }
        // pthread_exit(NULL);
}

void sendData(vector<Json::Value> &jsonVec) {
    string ip = "57.128.34.47";
    const char *serverIp = ip.c_str();
    int port = 22;
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(serverIp);
    sendSockAddr.sin_port = htons(port);
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0) {
        cerr << "Error connecting to server!" << endl;
        exit(1);
    }
    cout << "Connected to the server!" << endl;

    int vecSize = jsonVec.size();
    send(clientSd, &vecSize, sizeof(int), 0);
    for (auto json : jsonVec) {
        Json::FastWriter writer;
        string output = writer.write(json);
        const char* jsonStr = output.c_str();
        int jsonLen = strlen(jsonStr);
        send(clientSd, &jsonLen, sizeof(int), 0);
        send(clientSd, jsonStr, jsonLen, 0);
    }
    close(clientSd);
}