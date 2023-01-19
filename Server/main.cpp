#include <iostream>
#include <vector>


#include <jsoncpp/json/json.h>
#include "menu.cpp"
#include "src/retrieve.h"

using namespace std;

int main(){
    //menu();
    vector<Json::Value> res = retrieve();
    for(auto data: res){
        cout << res;
    }

    return 0;
}

