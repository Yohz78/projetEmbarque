#include <iostream>
#include <fstream>
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

#include "retrieve.h"
#include "../import_json/import_json.h"
#include "../create_db/create_db.h"

using namespace std;


/**
 * @brief This function initialize the server side for the tcp
 * 
 * @return int 
 */
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
    int reuse = 1;
    while(bindStatus < 0) {
        cerr << "serv_init: Error binding socket to local address" << endl;
        setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
    }

    cout << "serv_init: Valeur de serverSd = " << serverSd << endl;
    cout << " serv_init FIN" << endl;
    return serverSd;
}

/**
 * @brief This function transform a vector<string>  into a vector<json::value>.
 *  
 * @param string_data_vec 
 * @param jsonWrite 
 */
void jsoning(vector<string>& string_data_vec, vector<Json::Value>& jsonWrite){
    
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

    for (const string& s : string_data_vec) {
        Json::Value jsonVal;
        std::istringstream stream(s);
        if (Json::parseFromStream(builder, stream, &jsonVal, &errs)) {
            jsonWrite.push_back(jsonVal);
        }
        else {
            // Handle parse error
            std::cout << "Error parsing JSON: " << errs << std::endl;
        }
    }
}

/**
 * @brief This function write the json vector to a file
 * 
 * @param jsonWrite 
 */
void jsonToFile(vector<Json::Value>& jsonWrite){
    Json::StyledWriter writer;
    std::ofstream json_file("data.json", std::ios::app);
    for(auto jsonVal : jsonWrite)
    json_file << writer.write(jsonVal);
    json_file.close();
}

/**
 * @brief This function retrieves the data from the client. 
 * 
 * @param args 
 * @return void* 
 */
void* retrieve(void* args) { //int serverSd, int resNewSd, vector<Json::Value>& jsonWrite
    
    argsRetrieve* argsFunc = (argsRetrieve*)args;
    cout << " retrieve DEBUT" << endl;
    int serverSd = argsFunc->serverSd;
    int resNewSd = argsFunc->resNewSd;
    vector<Json::Value> jsonWrite;
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
    while(true){
        vector<string> string_data_vec;

        for(int i=0;i<3;i++){
            int data_received = recv(newSd, &msg, sizeof(msg), 0); // reception msg
            if(data_received > 3500){
            cout << "retrieve: data_received -----: " << data_received  << endl;
            cout << "retrieve: Valeur de message -: " << msg << endl;
            string_data_vec.push_back(msg);
                
                if((string_data_vec.size()-1) > 0){
                    cout << "------------------------------------------------" << endl;
                    cout << string_data_vec[(string_data_vec.size()-1)] << endl;
                    cout << "------------------------------------------------" << endl; 
                }            
                }else{
                    cout << "retrieve: <========================RIEN========================>" << endl;
                }
        
            cout << "retrieve: Nombre d'element: " << string_data_vec.size() << endl;
            sleep(3);
            cout << "retrieve: VALEUR DE i: " << i << endl;
            cout << endl;
        }
        cout << "JSONING THE DATA" << endl;
        for(auto dataVec: string_data_vec){
            cout << "----------------------" << endl;
            cout << dataVec << endl;
            cout << "----------------------" << endl;
        }
        jsoning(string_data_vec,jsonWrite);
        for(auto data: jsonWrite){
            cout << "----------------------" << endl;
            cout << data << endl;
            cout << "----------------------" << endl;
        }
        cout << "IMPORTING THE DATA" << endl;
        import_json_vector(jsonWrite);
        cout << "DATA IMPORTED" << endl;
        string_data_vec.clear();
        // ecriture dans le fichier data.json a partir de jsonWrite quand jsonWrite a une taille
        jsonToFile(jsonWrite);
        jsonWrite.clear();
    }
        return NULL;
}

/**
 * @brief This function closes the connection for the server
 * 
 * @param newSd 
 * @param serverSd 
 */
void serv_close(int& newSd, int& serverSd){
    cout << " serv_close DEBUT" << endl;
    close(newSd);
    close(serverSd);
    cout << " serv_close FIN" << endl;
}