#include <iostream>
#include <vector>
#include <pthread.h>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"

using namespace std;

int main(){
    argsRetrieve args(serv_init(),0);

    pthread_t retrieve_thread;
    pthread_create(&retrieve_thread, NULL,retrieve,(void*) &args); // retrieve();
    //menu();
    pthread_join(retrieve_thread, NULL);
    //delete args;
    serv_close(args.resNewSd,args.serverSd);
    return 0;
}

