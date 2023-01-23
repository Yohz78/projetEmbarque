#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <sstream>
#include <cstring>
#include <string>
#include <string.h>	//strlen
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
#include "src/send/send.h"

// #define INTERVALLE_RECUP 5
// #define INTERVALLE_ENVOI_SERVEUR 60



using namespace std;


int  master_serv_init(){
    cout << " serv_init DEBUT" << endl;
    int port = 8888;
    
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

void* watcher( void* socket_descriptor){

    PCA9685 pca(1,0x40);
    pca.init();
    int serverSd = (int) socket_descriptor;	
    char client_message[1+1];
    int mvt_tracker = 0;

    //Listen
	listen(serverSd , 1);

    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        cerr << "retrieve: Error accepting request from client!" << endl;
        exit(1);
    }
	
 	while(1){
        //Receive a message from client
        int read_size = recv(newSd , client_message , sizeof(client_message) , 0);
        if((strcmp(client_message,"1"))==0){
            pca.moveYellowFlag(90);
            cout << "read_and_write: Yellow flag to 90°" << endl;
            mvt_tracker=1;
        }else if((mvt_tracker==1)&&(read_size == -1)){
            pca.moveYellowFlag(180);
            cout << "read_and_write: Yellow flag to 180°" << endl;
        }else if((mvt_tracker==0)&&(read_size == -1)){
            pca.moveYellowFlag(0);
            cout << "read_and_write: Yellow flag to 0°" << endl;
        }
    }
    return NULL;
}

int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    int clientSd = send_init(); // DESCRIPTOR FOR TCP MASTER => SERVER
    int serverSD = master_serv_init(); // DESCRIPTOR FOR BALISE => MASTER

    pthread_t watcher_thread;

    //thread 1
    pthread_create(&watcher_thread,NULL, watcher, (void*) serverSD);

    //Read and write
    read_and_write(fd,clientSd);

    send_close(clientSd);
    serialClose(fd); // Ferme le port série
    return 0;
}