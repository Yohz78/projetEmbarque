#include <wiringSerial.h>
#include <pthread.h>
#include <unistd.h> // pour sleep
#include <iostream>

int fd;
int interval = 1; // Intervalle en secondes
//pthread_t thread;

void* read_sensor_data() {
    while (true) {
        serialPuts(fd, "all"); // Envoie la commande "all" à l'esclave
        char data[1000];
        int index = 0;
        while (serialDataAvail(fd)) {
            data[index] = serialGetchar(fd);
            index++;
        }
        data[index] = '\0';
        std::cout << "Données reçues : " << data << std::endl;
        sleep(interval); // Fait une pause pendant interval secondes
    }
}

int main() {
    fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    if (fd < 0) {
        std::cout << "Error: Unable to open UART device" << std::endl;
        return -1;
    }
    //pthread_create(&thread, NULL, read_sensor_data, NULL);
    //pthread_join(thread, NULL);
    read_sensor_data();
    serialClose(fd); // Ferme le port série
    return 0;
}
