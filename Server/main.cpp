#include <iostream>
#include <vector>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"

using namespace std;

int main(){
    //menu();
    int serverSd = serv_init();

    while(1){
        vector<Json::Value> res = retrieve(serverSd);
        for(auto data: res){
            cout << data;
        }
    }
    serv_close();
    return 0;
}

