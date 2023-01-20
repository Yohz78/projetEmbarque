#include <iostream>
#include <fstream>
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
};

typedef struct argsRetrieve argsRetrieve;

int  serv_init();
void jsoning(vector<string>&,vector<Json::Value>&);
void jsonToFile(vector<Json::Value>&);
void* retrieve(void*); // args: int serverSd, int resnewMD,vector<Json::Value>& jsonWrite
void serv_close(int&,int&);