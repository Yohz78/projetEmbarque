#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <typeinfo>

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

#include "../pca/pca.h"
#include "send.h"

#define INTERVALLE_RECUP 3
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
        cout << "read_sensor_data: Donnée reçue: "<< data << endl;
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
        cout << "read_and_write: string_data:  " << string_data << endl;
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
                cout << "read_and_write: Error parsing JSON" << endl;
            }
            if(pos_tracker==3){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "read_and_write: Blue flag to 45°" << endl;
            }else if(pos_tracker==2){
                pca.moveBlueFlag(135);
                pos_tracker=3;
                cout << "read_and_write: Blue flag to 135°" << endl;
            }else if(pos_tracker==0){
                pca.moveBlueFlag(45);
                pos_tracker=2;
                cout << "read_and_write: Blue flag to 45°" << endl;
            }
            //Yellow flag logic
            double presence = root["HCSR"]["mvt"].asDouble();
            if(presence==0 && mvt_tracker==1){
                pca.moveYellowFlag(180);
                cout << "read_and_write: Yellow flag to 180°" << endl;
            }
            if(presence==0 && mvt_tracker==0){
                pca.moveYellowFlag(0);
                cout << "read_and_write: Yellow flag to 0°" << endl;
            }    
            if(presence==1){
                pca.moveYellowFlag(90);
                cout << "read_and_write: Yellow flag to 90°" << endl;
                mvt_tracker=1;
            }    
            res.push_back(root);
            i++;
            if(i > 1){
                break;
            }
        }else{
            cout << "read_and_write:: No data available, Blue flag back to rest position." << endl;
            pca.moveBlueFlag(180);
            pos_tracker=0;
        } 
        sleep(INTERVALLE_RECUP);
        }
        // pthread_exit(NULL);
}

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
    if(status < 0) {
        cerr << "Error connecting to server!" << endl;
        exit(1);
    }
    cout << "Connected to the server!" << endl;
    return clientSd;
}

void sendData(int clientSd, vector<Json::Value> &jsonVec) {
    int vecSize = jsonVec.size();
    send(clientSd, &vecSize, sizeof(int), 0);
    char jsonStr[2000+1];
    char test[2000+1] = "TOTO";
    char testJson[2000+1] = "{\"temperature\": 22.860001}";
    // cout << "Type de jsonStr: DEBUT:" << typeid(jsonStr).name() << endl;

    send(clientSd, jsonStr,sizeof(jsonStr), 0);
    for (auto json : jsonVec) {
        Json::FastWriter writer;
        string output = writer.write(json);
        // cout << "sendData: Type de output: " << typeid(output).name() << endl;
        // cout << "sendData: OUTPUT: " << output << endl;
        strcat(jsonStr,output.c_str());
        // cout << "sendData: Type de jsonStr: APRES strcpy(jsonStr,output.c_str()) " << typeid(jsonStr).name() << endl;
        // cout << "sendData: Contenu de JsonStr" << jsonStr << endl;
        
    }
    // {"date":"2023-01-19T17:18:25","BME": {"temperature": 22.860001,"pressure": 999.383728,"humidity": 28.709961},
    // "HCSR": {"mvt": 0},"HMC": {"x": 18146,"y": -358,"z":-1587}}

    // cout << "-------------------------------------------" << endl;
    // cout << "sendData: TABLEAU JSONTSTR:   " << jsonStr << endl;
    // cout << "-------------------------------------------" << endl;
    //send(clientSd, jsonStr, sizeof(jsonStr), 0);
    //send(clientSd, "TOTO", sizeof("TOTO") ,0);
    send(clientSd, test, sizeof(test) ,0);
    send(clientSd, testJson, sizeof(testJson), 0);
}

void send_close(int clientSd){
    close(clientSd);
}