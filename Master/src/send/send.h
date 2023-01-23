#ifndef SEND_H
#define SEND_H

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

#include "../pca/pca.h"

// Collect data from balisa
std::string read_sensor_data(int); 
// Manage blue flag animation and send data to server via TCP
void read_and_write(int,int); 
// Prepare and finalize connection to server
int send_init();
// Send data to server
void sendData(int,std::string&);
// End communication with the server
void send_close(int);

#endif //SEND_H