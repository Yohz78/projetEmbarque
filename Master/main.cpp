#include <wiringSerial.h>
#include <pthread.h>
#include <unistd.h> // pour sleep
#include <iostream>

int fd;
int interval = 1; // Intervalle en secondes
pthread_t thread;

void* read_sensor_data(void *args) {
    while (true) {
        char data[10000];
        int index = 0;
        serialPuts(fd, "all"); 
        while (serialDataAvail(fd) > 0) {
            int c = serialGetchar(fd);
            if (c < 0) {
                std::cout << "Error: Unable to receive data over UART" << std::endl;
            }
            std::cout << "Des données sont présentes : " << data << std::endl;
            data[index] = c;
            index++;
        }
        data[index] = '\0';
        std::cout << "Données reçues : " << data << std::endl;
        std::cout << "Données reçues : " << data[0] << std::endl;
        sleep(interval); // Fait une pause pendant interval secondes
    }
}


void lecture(int fd){
    std::string commande = "all";
    std::cout << "On envoie la commande " + commande + " au slave" << std::endl;
    serialPuts(fd,commande.c_str());
    sleep(1);
    if (serialDataAvail(fd) > 0) { // Vérifie s'il y a des données disponibles pour lecture
        std::string data;
            while (serialDataAvail(fd) > 0) {
                char c = serialGetchar(fd);
                data += c;
            }
        std::cout << "Données reçues : " << data << std::endl;
        }else{
            std::cout << "J'ai pas trouvé de données chef !!! " << std::endl;    
    } 
}

void ecriture_rand(){
    int data = rand() % 100; // Génère un nombre aléatoire entre 0 et 99
    std::string data_str = std::to_string(data); // Convertit le nombre en chaîne de caractères
    serialPuts(fd, data_str.c_str()); // Envoie les données sur le port série */ 
    std::cout << "Données envoyées : " << data_str << std::endl;    
}


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
        
        //ecriture_rand();
        int data = rand() % 100; // Génère un nombre aléatoire entre 0 et 99
        std::string data_str = std::to_string(data); // Convertit le nombre en chaîne de caractères
        serialPuts(fd, data_str.c_str()); // Envoie les données sur le port série */ 
        std::cout << "Données envoyées : " << data_str << std::endl; 
        
        
        sleep(1); // Fait une pause pendant 1 seconde
    }

    serialClose(fd); // Ferme le port série
    return 0;
}

/*
int main() {
    fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    pthread_create(&thread, NULL, read_sensor_data, NULL);
    pthread_join(thread, NULL);
    serialClose(fd); // Ferme le port série
    return 0;
}
*/