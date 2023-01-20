#include <iostream>
#include <vector>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"

using namespace std;

int main(){
    //menu();
    int serverSd = serv_init();
    int resNewSd = 0;
    vector<string> res;
    resNewSd = retrieve(serverSd,res);
    for(auto data: res){
        cout << "------------------------------------------------" << endl;
        cout << data << endl;
        cout << "------------------------------------------------" << endl;
    }
    res.clear();
    serv_close(resNewSd,serverSd);
    return 0;
}

