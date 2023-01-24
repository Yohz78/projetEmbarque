#include <iostream>
#include <fstream>

#include "menu.h"
#include "../statements/statements.h"


/**
 * @brief This function is the menu 
 * 
 */
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
                    afficheAllData();
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
                    std::cout << "Au revoir!" << std::endl;
                    exit(0);
                default:
                    std::cout << "Option non valide, veuillez réessayer." << std::endl;
                    break;
        }
    }
    }
