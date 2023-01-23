#include <iostream>
#include <vector>
#include <pthread.h>


#include <jsoncpp/json/json.h>
#include "src/menu/menu.h"
#include "src/retrieve/retrieve.h"
#include "src/create_db/create_db.h"

using namespace std;

int main(){
    argsRetrieve args(serv_init(),0);
    createDatabase("data.db");

    pthread_t retrieve_thread;
    pthread_create(&retrieve_thread, NULL,retrieve,(void*) &args); // retrieve();
    // pthread_join(retrieve_thread, NULL);
    pthread_detach(retrieve_thread);
    menu();
    serv_close(args.resNewSd,args.serverSd);
    return 0;
}

// #include <fstream>
// #include <iostream>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// int main() {
//     // lire les données à partir du fichier JSON
//     std::ifstream file("data.json");
//     json root;
//     file >> root;

//     // itérer sur les données
//     for (auto& measurement : root) {
//         std::cout << "Date de la mesure : " << measurement["date"].get<std::string>() << std::endl;
//         std::cout << "Données du capteur BME : " << std::endl;
//         std::cout << "Température : " << measurement["BME"]["temperature"].get<std::string>() << std::endl;
//         std::cout << "Pression : " << measurement["BME"]["pression"].get<std::string>() << std::endl;
//         std::cout << "Humidité : " << measurement["BME"]["humidity"].get<std::string>() << std::endl;

//         double presence = measurement["HCSR"]["mvt"].get<double>();
//         if(presence==1){
//             std::cout << "Capteur HCSR : " << "Présence détectée"<< std::endl;
//         }else{
//             std::cout << "Capteur HCSR : " << "Pas de présence détectée"<< std::endl;
//         }

//         std::cout << "Données du capteur HMC : " << std::endl;
//         std::cout << "Coordonnée X : " << measurement["HMC"]["x"].get<std::string>() << std::endl;
//         std::cout << "Coordonnée Y : " << measurement["HMC"]["y"].get<std::string>() << std::endl;
//         std::cout << "Coordonnée Z : " << measurement["HMC"]["z"].get<std::string>() << std::endl;
//     }
//     return 0;
// }
