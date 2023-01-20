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


int balise_send_init();
void* balise_sendData(void*);
void balise_send_close(int);

#endif //SEND_H