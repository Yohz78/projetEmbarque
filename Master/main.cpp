#include <wiringSerial.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <pthread.h>
#include <jsoncpp/json/json.h>

#include "send.cpp"


int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    vector<Json::Value> res;
    read_and_write(fd,res);

    for(auto data: res){
        std::cout << data;
    }

    std::cout << "-------------------------------ENVOI----------------------------------" << std::endl;
    sendData(res);
    res.clear();
    std::cout << "-------------------------------FIN ENVOI----------------------------------" << std::endl;
    
    /*pthread_t thread_rw, thread_send;

    pthread_create(&thread_rw, NULL, read_and_write, &fd, &res);
    pthread_create(&thread_send, NULL,send_boost, &fd, &res);
    pthread_join(thread_rw, NULL);
    pthread_join(thread_send, NULL);*/ 

    // C  est ici que l'on va avoir besoin des threads, un qui recupere de la data et un qui envoie, les deux a differents intervalles
    //read_and_write(fd, res);

    serialClose(fd); // Ferme le port série
    return 0;
}