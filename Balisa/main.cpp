#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <wiringSerial.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <ctime>
#include <stdint.h>
#include <unistd.h>
#include <iomanip>
#include<sys/socket.h>
#include<arpa/inet.h>	
#include "src/handler/handler.h"
#include "src/send/send.h"
#include<pthread.h> //for threading , link with lpthread

//#include "src/bme/bme.h"

/**
 * @brief This function gets a command input from the Master and execute the command. 
 * 
 * @param[in] fd 
 * @param[in] Handler handler 
 */
void* loop(void*fd) {
    Handler handler;
    int new_fd=(int)fd;
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss1;
    oss1 << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    std::string iso_time = oss1.str();
    std::string sep = "\"";
    std::ostringstream oss;
    std::cout << "---------------------DATAS---------------------" << std::endl;
    oss << "{\"date\":" + sep + iso_time + sep;
    oss << ",";
    oss << handler.getBME().harvestDataAndRun();
    oss << ",";
    oss << handler.getHCSR().checkMotion(); 
    oss << ",";
    oss << handler.getHMCvalue();
    oss << "}";
    serialPuts(new_fd,oss.str().c_str());
    std::cout << "Donnees envoyees:" << std::endl;
    std::cout << oss.str();
    std::cout << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    sleep(3);
    return 0;
}

    // void *connection_handler(void *socket_desc)
    // {
    //     //Get the socket descriptor
    //     int sock = (int) socket_desc;
        
    //     // Send some messages to the client
    //     Handler handler;
    //     while(1){
    //     char message[1+1];
    //     std::string motion = handler.getHCSR().watcherMotion();
    //     strcpy(message, motion.c_str());
    //     if((strcmp(message,"1"))==0){
    //         write(sock , message , strlen(message));
    //     }
    //     }
    //     return 0;
    // }

int main(){

    int clientSd = balise_send_init();
	   
    pthread_t tcp_thread;
    if(pthread_create(&tcp_thread, NULL, balise_sendData, (void *) clientSd) < 0){
            perror("could not create thread");
            return 1;
    }
        
    //Now join the thread , so that we dont terminate before the thread
    pthread_join(tcp_thread , NULL);
    puts("Handler assigned");
    
    //TX/RX PART

    pthread_t data_thread;
    int fd;
    while((fd = serialOpen("/dev/ttyAMA0", 9600))>0){
        if (fd < 0) {
            std::cout << "Error: Unable to open UART device" << std::endl;
            return -1;
        }
        if(pthread_create(&data_thread, NULL, loop, (void *) fd) < 0){
            std::cout <<"could not create thread"<< std::endl;
            return 1;
        }
        pthread_join(data_thread, NULL);
    }
    serialClose(fd);
    return 0;
}

// int main() {
//     int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyS0 à 9600 bauds
//     if (fd < 0) {
//         std::cout << "Error: Unable to open UART device" << std::endl;
//         return -1;
//     }

//     Handler handler; // Creation du wrapper

//     // thread 1
//     while (true) {
//         loop(fd, &handler);
//         sleep(3); // Fait une pause pendant 1 seconde  
//     }

//     // thread 2 
//     watcher(&handler);

    
//     serialClose(fd); // Ferme le port série
//     return 0;
// }
