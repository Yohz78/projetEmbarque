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
#include <jsoncpp/json/json.h>

using namespace std;

int  serv_init(){
    cout << " serv_init DEBUT" << endl;
    int port = 1500;
    
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSd < 0) {
        cerr << "serv_init: Error establishing the server socket" << endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    cout << "serv_init: Valeur de bindStatus: " << bindStatus << endl;

    if (bindStatus < 0) {
        cerr << "serv_init: Error binding socket to local address" << endl;
        close(serverSd);
        //close(newSd);
        exit(0);
    }

    cout << "serv_init: Valeur de serverSd = " << serverSd << endl;
    cout << " serv_init FIN" << endl;
    return serverSd;
}

int retrieve(int serverSd, vector<string>& jsonVec) {
    cout << " retrieve DEBUT" << endl;
    char msg[4000];
    cout << "retrieve: Waiting for a client to connect..." << endl;
    cout << "retrieve: LISTENING ON" << endl;
    listen(serverSd, 5);
    cout << "retrieve: LISTENING OVER" << endl;
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    cout << "retrieve: NEWSD DEBUT" << endl;
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    cout << "retrieve: NEWSD FIN" << endl;

    if (newSd < 0) {
        cerr << "retrieve: Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "retrieve: Connected with client!" << endl;

    //while(1){
        int vecSize = 1;
        recv(newSd, &vecSize, sizeof(int), 0);
        for (int i = 0; i < vecSize; i++) {
            Json::Value jsonVal;
            Json::Reader reader;
            recv(newSd, &msg, sizeof(msg), 0); // reception msg
            cout << "Valeur de message : " << msg << endl;
            if (reader.parse(msg, jsonVal)) {
                jsonVec.push_back(jsonVal);
            }else{
                cout << "retrieve: reader.parse(msg, jsonVal): "
                 <<reader.parse(msg, jsonVal)  << endl;
            }
        }
        cout << "retrieve: Nombre d'element: " << jsonVec.size() << endl;

        // if(!jsonVec.empty()){
        //     cout << "retrieve: Received vector of Json objects from client:" << endl;
        //     for (auto json : jsonVec) {
        //         cout << json << endl;
        //     }
        // }else{
        //     cout << "retrieve: JSONVEC is empty" << endl;
        // }
        cout << " retrieve FIN" << endl;
        return newSd;
    //}
}

void serv_close(int& newSd, int& serverSd){
    cout << " serv_close DEBUT" << endl;
    close(newSd);
    close(serverSd);
    cout << " serv_close FIN" << endl;
}