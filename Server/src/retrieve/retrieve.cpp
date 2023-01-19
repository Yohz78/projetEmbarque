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
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <json/json.h>

using namespace std;

vector<Json::Value> retrieve() {
    int port = 21;
    char msg[1500];
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSd < 0) {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));

    if (bindStatus < 0) {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    cout << "Waiting for a client to connect..." << endl;
    listen(serverSd, 5);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);

    if (newSd < 0) {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;

    int vecSize;
    recv(newSd, &vecSize, sizeof(int), 0);
    vector<Json::Value> jsonVec;
    for (int i = 0; i < vecSize; i++) {
        Json::Value jsonVal;
        Json::Reader reader;
        recv(newSd, &msg, sizeof(msg), 0);
        if (reader.parse(msg, jsonVal)) {
            jsonVec.push_back(jsonVal);
        }
    }
    cout << "Received vector of Json objects from client:" << endl;
    for (auto json : jsonVec) {
        cout << json << endl;
    }

    close(newSd);
    close(serverSd);
    return jsonVec;
}
