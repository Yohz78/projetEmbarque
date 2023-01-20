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
#include "src/send/send.h"

// #define INTERVALLE_RECUP 5
// #define INTERVALLE_ENVOI_SERVEUR 60



using namespace std;


int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    int clientSd = send_init();
    while(1){
    vector<string> res;
    cout << "-----------main: res size BEGIN = " << res.size() << endl;
    read_and_write(fd,res);

    std::cout << "-------------------------------ENVOI----------------------------------" << std::endl;
    sendData(clientSd,res);
    cout << "-----------main: res size AFTER = " << res.size() << endl;
    res.clear();
    std::cout << "-------------------------------FIN ENVOI----------------------------------" << std::endl;
    }
    /*pthread_t thread_rw, thread_send;

    pthread_create(&thread_rw, NULL, read_and_write, &fd, &res);
    pthread_create(&thread_send, NULL,send_boost, &fd, &res);
    pthread_join(thread_rw, NULL);
    pthread_join(thread_send, NULL);*/ 

    // C  est ici que l'on va avoir besoin des threads, un qui recupere de la data et un qui envoie, les deux a differents intervalles
    //read_and_write(fd, res);

    send_close(clientSd);
    serialClose(fd); // Ferme le port série
    return 0;
}