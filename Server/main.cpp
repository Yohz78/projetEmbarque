#include <iostream>
#include <vector>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"

using namespace std;

int main(){
    //menu();
    vector<Json::Value> res = retrieve();
    for(auto data: res){
        cout << data;
    }

    return 0;
}

