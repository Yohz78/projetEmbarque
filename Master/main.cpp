#include <wiringSerial.h>
#include <unistd.h> // pour sleep
#include <iostream>

int main() {
    int fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }

    std::string commande = "all";


    while (true) {
        std::cout << "Valeur de fd=====================> " << fd << std::endl;
        std::cout << "Valeur de serialDataAvail(fd)====> " << serialDataAvail(fd) << std::endl;
        
        //ecriture_rand;
        int data = rand() % 100; // Génère un nombre aléatoire entre 0 et 99
        std::string data_str = std::to_string(data); // Convertit le nombre en chaîne de caractères
        serialPuts(fd, data_str.c_str()); // Envoie les données sur le port série */ 
        std::cout << "Données envoyées : " << data_str << std::endl; 
        
        
        sleep(1); // Fait une pause pendant 1 seconde
    }

    serialClose(fd); // Ferme le port série
    return 0;
}

