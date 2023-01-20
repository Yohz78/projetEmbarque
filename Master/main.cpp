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

void watcher(){
	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	char *message, client_message[2000]="niet";
    int read_size=0; 

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Watcher : Could not create socket");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		puts("Watcher : bind failed");
	}
	puts("Watcher : bind done");
	
	//Listen
	listen(socket_desc , 3);
	
 	while(1)
 {
 
	//Accept and incoming connection
	puts("Watcher : Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (new_socket<0){
		perror("Watcher : accept failed");
	}

  
    //Receive a message from client
	
    while ((read_size = recv(new_socket , client_message , 2000 , 0)) > 0){
    printf("\nread_size %d", read_size);
		//Send the message back to client
		write(new_socket , client_message , strlen(client_message));
	}
    
    //use client message to check for motion and move yellow flag if needed.
    puts(client_message);
    PCA9685 pca(1,0x40);
    pca.init();
    int mvt_tracker = 0;
    if(strcmp(client_message,"0")==0 && mvt_tracker==1){
        pca.moveYellowFlag(180);
        cout << "read_and_write: Yellow flag to 180°" << endl;
    }
    if(strcmp(client_message,"0")==0 && mvt_tracker==0){
        pca.moveYellowFlag(0);
        cout << "read_and_write: Yellow flag to 0°" << endl;
    }    
    if(strcmp(client_message,"1")==0){
        pca.moveYellowFlag(90);
        cout << "read_and_write: Yellow flag to 90°" << endl;
        mvt_tracker=1;
    }    
 
 	puts("Connection accepted");

	if(read_size == 0){
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1){
		perror("recv failed");
	}
  }
}

int main() {

    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    int clientSd = send_init();

    //thread 1
    while(1){
    std::cout << "-------------------------------ENVOI----------------------------------" << std::endl;
    read_and_write(fd,clientSd);
    std::cout << "-------------------------------FIN ENVOI----------------------------------" << std::endl;
    }

    //thread 2
    watcher();


    send_close(clientSd);
    serialClose(fd); // Ferme le port série
    return 0;
}