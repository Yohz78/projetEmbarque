#include <iostream>
#include <jsoncpp/json/json.h>

#include "menu.h"
using namespace std;

// std::istringstream json_stream(jsonString);
// Json::Value root;
// Json::CharReaderBuilder builder;
// JSONCPP_STRING errs;
// bool parsingSuccessful = Json::parseFromStream(builder,
//                                                json_stream,
//                                                &root,
//                                                &errs);

void afficheData(Json::Value root){
    std::cout << "Date de la mesure : " << root["date"].asString() << std::endl;
    std::cout << "Données du capteur BME : " << std::endl;
    std::cout << "Température : " << root["BME"]["temperature"].asString() << std::endl;
    std::cout << "Pression : " << root["BME"]["pression"].asString() << std::endl;
    std::cout << "Humidité : " << root["BME"]["humidity"].asString() << std::endl;
    double presence = root["mvt"].asDouble();
    if(presence==1){
        std::cout << "Capteur HCSR : " << "Présence détectée"<< std::endl;
    }else{
        std::cout << "Capteur HCSR : " << "Pas de présence détectée"<< std::endl;
    }
    std::cout << "Données du capteur HMC : " << std::endl;
    std::cout << "Coordonnée X : " << root["HMC"]["x"].asString() << std::endl;
    std::cout << "Coordonnée Y : " << root["HMC"]["y"].asString() << std::endl;
    std::cout << "Coordonnée Z : " << root["HMC"]["z"].asString() << std::endl;
}

void afficheBME(Json::Value root){
    std::cout << "Date de la mesure : " << root["date"].asString() << std::endl;
    std::cout << "Données du capteur BME : " << std::endl;
    std::cout << "Température : " << root["BME"]["temperature"].asString() << std::endl;
    std::cout << "Pression : " << root["BME"]["pression"].asString() << std::endl;
    std::cout << "Humidité : " << root["BME"]["humidity"].asString() << std::endl;    
}

void afficheHCSR(Json::Value root){
    std::cout << "Date de la mesure : " << root["date"].asString() << std::endl;
    double presence = root["mvt"].asDouble();
    if(presence==1){
        std::cout << "Capteur HCSR : " << "Présence détectée"<< std::endl;
    }else{
        std::cout << "Capteur HCSR : " << "Pas de présence détectée"<< std::endl;
    }  
}

void afficheHMC(Json::Value root){
    std::cout << "Date de la mesure : " << root["date"].asString() << std::endl;
    std::cout << "Données du capteur HMC : " << std::endl;
    std::cout << "Coordonnée X : " << root["HMC"]["x"].asString() << std::endl;
    std::cout << "Coordonnée Y : " << root["HMC"]["y"].asString() << std::endl;
    std::cout << "Coordonnée Z : " << root["HMC"]["z"].asString() << std::endl;   
}

void menu(){
    std::string jsonString= "{\"date:\" \"2023-01-17T15:38:16\",\"BME\": {\"temperature\": \"22.250000\",\"pression\": \"977.488647\",\"humidity\": \"29.601562\"},\"HCSR\": {\"mvt\": \"0\"},\"HMC\": {\"x\": \"18146\"\"y\": \"-358\",\"z\": \"-1587\"}}";
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString, root);
    if(!parsingSuccessful){
        std::cout << "Error parsing JSON" << std::endl;
    }else{
        cout << "Bonjour utilisateur!" << endl;
        while (true) {
            cout << "Veuillez choisir une option:" << endl;
            cout << "1. Afficher les données" << endl;
            cout << "2. Afficher les données du capteur BME" << endl;
            cout << "3. Afficher les données du capteur HCSR" << endl;
            cout << "4. Afficher les données du capteur HMC" << endl;
            cout << "0. Quitter" << endl;

            int choix;
            cin >> choix;

            switch (choix) {
                case 1:
                    afficheData(root);
                    break;
                case 2:
                    afficheBME(root);
                    break;
                case 3:
                    afficheHCSR(root);
                    break;
                case 4:
                    afficheHMC(root);
                    break;
                case 0:
                    cout << "Au revoir!" << endl;
                    exit(0);
                default:
                    cout << "Option non valide, veuillez réessayer." << endl;
                    break;
        }
    }
    }
}

