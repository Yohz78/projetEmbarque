#include <wiringPiI2C.h>

const int PCA9685_ADDRESS = 0x40; // Adresse I2C de la PCA9685
const int PCA9685_MODE1 = 0x00; // Registre de configuration du mode 1
const int PCA9685_PRESCALE = 0xFE; // Registre de configuration de la pré-échelle
const int SERVO_1 = 0; // Connecteur du servomoteur 1
const int SERVO_2 = 1; // Connecteur du servomoteur 2

int fd;

// Fonction pour initialiser la carte PCA9685
void initPCA9685() {
    fd = wiringPiI2CSetup(PCA9685_ADDRESS);
    wiringPiI2CWriteReg8(fd, PCA9685_MODE1, 0x00); // Réinitialise la carte
    // Réglage de la fréquence PWM à 50 Hz
    int prescale = 25000000;
    prescale /= 4096;
    prescale /= 50;
    prescale -= 1;
    int oldmode = wiringPiI2CReadReg8(fd, PCA9685_MODE1);
    int newmode = (oldmode & 0x7F) | 0x10;
    wiringPiI2CWriteReg8(fd, PCA9685_MODE1, newmode);
    wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE, prescale);
    wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode);
    wiringPiI2CWriteReg8(fd, PCA9685_MODE1, oldmode | 0x80);
}

// Fonction pour définir l'angle de rotation d'un servomoteur
void setServo(int servo, int angle) {
    int on = 0;
    int off = angle * 4096 / 180;
    wiringPiI2CWriteReg8(fd, LED0_ON_L + 4 * servo, on & 0xFF);
    wiringPiI2CWriteReg8(fd, LED0_ON_H + 4 * servo, on >> 8);
    wiringPiI2CWriteReg8(fd, LED0_OFF_L + 4 * servo, off & 0xFF);
    wiringPiI2CWriteReg8(fd, LED0_OFF_H + 4 * servo, off >> 8);
}

int main() {
    initPCA9685();
    setServo(SERVO_1, 90); // Tourne le servomoteur 1 à 90 degrés
    setServo(SERVO_2, 180); // Tourne le servomoteur 2 à 180 degrés
    return 0;
}
