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
//#include "src/bme/bme.h"

/**
 * @brief This function gets a command input from the Master and execute the command. 
 * 
 * @param[in] fd 
 * @param[in] Handler handler 
 */
void loop(int fd,Handler* handler) {
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
            oss << handler->getBME().harvestDataAndRun();
            oss << ",";
            oss << handler->getHCSR().checkMotion(); 
            oss << ",";
            oss << handler->getHMCvalue();
            oss << "}";
            serialPuts(fd,oss.str().c_str());
            std::cout << "Donnees envoyees:" << std::endl;
            std::cout << oss.str();
            std::cout << std::endl;
            std::cout << "----------------------------------------------" << std::endl;
}

void watcher(Handler* handler){
    int socket_desc;
	struct sockaddr_in server;
	char message[2000] , server_reply[2000];
    //Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Watcher : Could not create socket");
		
	server.sin_addr.s_addr = inet_addr("192.168.68.63");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("Watcher : connect error");
	}
	
	puts("Watcher : Connected\n");
	
    while(true){
        //send motion value
        std::string messageStr = handler->getHCSR().watcherMotion();
        strcpy(message,messageStr.c_str());
        if( send(socket_desc , message , strlen(message) , 0) < 0){
            puts("Watcher : Send failed");
        }
        puts("Watcher : Data Send\n");
        
        //Receive a reply from the server
        if (recv(socket_desc, server_reply , 2000 , 0) < 0){
            puts("Watcher : recv failed");
        }
        puts("Watcher : Reply received\n");
        puts(server_reply);
    }
}

int main() {
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyS0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    Handler handler; // Creation du wrapper

    // thread 1
    while (true) {
        loop(fd, &handler);
        sleep(3); // Fait une pause pendant 1 seconde  
    }

    // thread 2 
    watcher(&handler);

    
    serialClose(fd); // Ferme le port série
    return 0;
}