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

#include "send.h"
#include "../handler/handler.h"

#define INTERVALLE_RECUP 3
#define INTERVALLE_ENVOI_SERVEUR 60

using namespace std;

/**
 * @brief This function read the sensor data from the slave over TX/RX communication
 * 
 * @param fd 
 * @return string 
 */

int balise_send_init(){
    // string ip = "90.86.59.230";
    string ip = "192.168.1.171";
    const char *serverIp = ip.c_str();
    int port = 1600;
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

void* balise_sendData(void* clientSd) {
    char message[1+1];
    int clientSD = (int) clientSd;
    Handler handler;
    while(1){
        std::string motion = handler.getHCSR().watcherMotion();
        strcpy(message, motion.c_str());
        if((strcmp(message,"1"))==0){
            send(clientSD, &message, sizeof(message) ,0);
        }
    return NULL;
}
}

void balise_send_close(int clientSd){
    close(clientSd);
}