#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <json/json.h>

using namespace std;

void sendData(vector<Json::Value> &jsonVec) {
    char *serverIp = "57.128.34.47";
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