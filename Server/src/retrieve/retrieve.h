#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

struct argsRetrieve {
    int serverSd;
    int resNewSd;
    argsRetrieve(int serverSd, int resNewSd) : serverSd(serverSd), resNewSd(resNewSd) {}
}argsRetrieve;

int  serv_init();
void jsoning(vector<string>&,vector<Json::value>&);
void jsonToFile(vector<Json::value>&);
void* retrieve(void*); // args: int serverSd, int resnewMD,vector<Json::value>& jsonWrite
void serv_close(int&,int&);