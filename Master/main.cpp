#include <wiringSerial.h>
#include <pthread.h>
#include <unistd.h> // pour sleep

int fd;
int interval = 1; // Intervalle en secondes

void* read_sensor_data(void* args) {
    while (true) {
        serialPuts(fd, "all"); // Envoie la commande "all" à l'esclave
        char data[100];
        int index = 0;
        while (serialDataAvail(fd)) {
            data[index] = serialGetChar(fd);
            index++;
        }
        data[index] = '\0';
        std::cout << "Données reçues : " << data << std::endl;
        sleep(interval); // Fait une pause pendant interval secondes
    }
}

int main() {
    fd = serialOpen("/dev/ttyAMA0", 9600); // Ouvre le port série sur /dev/ttyAMA0 à 9600 bauds
    pthread_create(&thread, NULL, read_sensor_data, NULL);
    pthread_join(thread, NULL);
    serialClose(fd); // Ferme le port série
    return 0;
}
