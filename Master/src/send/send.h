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

std::string read_sensor_data(int);
void read_and_write(int, vector<Json::Value>&);
int send_init();
void sendData(vector<Json::Value>&);
void send_close(int);

#endif //SEND_H