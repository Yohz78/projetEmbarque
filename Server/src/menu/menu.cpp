#include <iostream>
#include <jsoncpp/json/json.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "menu.h"
using json = nlohmann::json;

void afficheData(){
    
    std::ifstream file("data.json");
    json root;
    file >> root;
    for (auto& measurement : root) {
        std::cout << "Date de la mesure : " << measurement["date"].get<std::string>() << std::endl;
        std::cout << "Données du capteur BME : " << std::endl;
        std::cout << "Température : " << measurement["BME"]["temperature"].get<std::string>() << std::endl;
        std::cout << "Pression : " << measurement["BME"]["pression"].get<std::string>() << std::endl;
        std::cout << "Humidité : " << measurement["BME"]["humidity"].get<std::string>() << std::endl;

        double presence = measurement["HCSR"]["mvt"].get<double>();
        if(presence==1){
            std::cout << "Capteur HCSR : " << "Présence détectée"<< std::endl;
        }else{
            std::cout << "Capteur HCSR : " << "Pas de présence détectée"<< std::endl;
        }

        std::cout << "Données du capteur HMC : " << std::endl;
        std::cout << "Coordonnée X : " << measurement["HMC"]["x"].get<std::string>() << std::endl;
        std::cout << "Coordonnée Y : " << measurement["HMC"]["y"].get<std::string>() << std::endl;
        std::cout << "Coordonnée Z : " << measurement["HMC"]["z"].get<std::string>() << std::endl;
}
file.close();
}

void afficheBME(){
    std::ifstream file("data.json");
    json root;
    file >> root;
    for (auto& measurement : root) {
        std::cout << "Date de la mesure : " << measurement["date"].get<std::string>() << std::endl;
        std::cout << "Données du capteur BME : " << std::endl;
        std::cout << "Température : " << measurement["BME"]["temperature"].get<std::string>() << std::endl;
        std::cout << "Pression : " << measurement["BME"]["pression"].get<std::string>() << std::endl;
        std::cout << "Humidité : " << measurement["BME"]["humidity"].get<std::string>() << std::endl;
    }
    file.close();

}

void afficheHCSR(){
    std::ifstream file("data.json");
    json root;
    file >> root;
    for (auto& measurement : root) {
        double presence = measurement["HCSR"]["mvt"].get<double>();
        if(presence==1){
            std::cout << "Capteur HCSR : " << "Présence détectée"<< std::endl;
        }else{
            std::cout << "Capteur HCSR : " << "Pas de présence détectée"<< std::endl;
        }
    }
    file.close();

}

void afficheHMC(){
    std::ifstream file("data.json");
    json root;
    file >> root;
    for (auto& measurement : root) {
        std::cout << "Données du capteur HMC : " << std::endl;
        std::cout << "Coordonnée X : " << measurement["HMC"]["x"].get<std::string>() << std::endl;
        std::cout << "Coordonnée Y : " << measurement["HMC"]["y"].get<std::string>() << std::endl;
        std::cout << "Coordonnée Z : " << measurement["HMC"]["z"].get<std::string>() << std::endl;
    }  
    file.close();
}

void menu(){
        std::cout << "Bonjour utilisateur!" << std::endl;
        while (true) {
            std::cout << "Veuillez choisir une option:" << std::endl;
            std::cout << "1. Afficher les données" << std::endl;
            std::cout << "2. Afficher les données du capteur BME" << std::endl;
            std::cout << "3. Afficher les données du capteur HCSR" << std::endl;
            std::cout << "4. Afficher les données du capteur HMC" << std::endl;
            std::cout << "0. Quitter" << std::endl;

            int choix;
            std::cin >> choix;

            switch (choix) {
                case 1:
                    afficheData();
                    break;
                case 2:
                    afficheBME();
                    break;
                case 3:
                    afficheHCSR();
                    break;
                case 4:
                    afficheHMC();
                    break;
                case 0:
                    cout << "Au revoir!" << endl;
                    exit(0);
                default:
                    std::cout << "Option non valide, veuillez réessayer." << endl;
                    break;
        }
    }
    }
