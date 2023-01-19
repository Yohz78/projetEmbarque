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
    int port = 1500;
    
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
    cout << "Valeur de bindStatus: " << bindStatus << endl;

    if (bindStatus < 0) {
        cerr << "Error binding socket to local address" << endl;
        close(serverSd);
        //close(newSd);
        exit(0);
    }

    cout << "Valeur de serverSd = " << serverSd << endl;
    return serverSd;
}

void retrieve(int serverSd, int& resNewSd, vector<Json::Value>& jsonVec) {
    //char msg[1500];
    cout << "retrieve: Waiting for a client to connect..." << endl;
    listen(serverSd, 5);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    resNewSd = newSd;

    if (newSd < 0) {
        cerr << "retrieve: Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "retrieve: Connected with client!" << endl;

    //while(1){
        int vecSize = 2;
        recv(newSd, &vecSize, sizeof(int), 0);
        for (int i = 0; i < vecSize; i++) {
            Json::Value jsonVal;
            Json::Reader reader;
            recv(newSd, &msg, sizeof(msg), 0);
            if (reader.parse(msg, jsonVal)) {
                jsonVec.push_back(jsonVal);
            }

            // istringstream json_stream(string_data);
            // Json::Value root;
            // Json::CharReaderBuilder builder;
            // JSONCPP_STRING errs;
            // bool parsingSuccessful = Json::parseFromStream(builder,
            //                                            json_stream,
            //                                            &root,
            //                                            &errs);

            // Json::Value jsonVal;
            // Json::CharReaderBuilder builder;
            // std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            // std::string msg;
            // recv(newSd, &msg, sizeof(msg), 0);

            // if (reader->parse(msg.c_str(), msg.c_str() + msg.size(), &jsonVal, nullptr)) {
            //     jsonVec.push_back(jsonVal);
            // }
        }
        if(!jsonVec.empty()){
            cout << "retrieve: Received vector of Json objects from client:" << endl;
            for (auto json : jsonVec) {
                cout << json << endl;
            }
        }else{
            cout << "retrieve: JSONVEC is empty" << endl;
        }
    //}
}

void serv_close(int& newSd, int& serverSd){
    close(newSd);
    close(serverSd);
}