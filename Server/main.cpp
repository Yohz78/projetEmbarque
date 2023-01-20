#include <iostream>
#include <vector>
#include <pthread.h>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"

using namespace std;



int main(){
    struct argsRetrieve args(serv_init(),0);

    

    pthread_t retrieve_thread;
    pthread_create(&retrieve_thread, NULL,retrieve,(void*) args); // retrieve();
    //menu();
    res.clear();
    pthread_join(retrieve_thread, NULL);
    serv_close(args.resNewSd,args.serverSd);
    return 0;
}

