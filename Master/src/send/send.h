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

// Récupère les données venant de la balise
std::string read_sensor_data(int); 
// Gère le drapeau bleu et transmet les données via TCP/IP
void read_and_write(int,int); 
// Prépare et effectue la connexion TCP/IP au serveur
int send_init();
// Transmet les données au serveur
void sendData(int,std::string&);
// Ferme la communication avec le serveur
void send_close(int);

#endif //SEND_H