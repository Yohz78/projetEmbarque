#include "pca.h"

void PCA9685::init(){
    fd = wiringPiI2CSetup(this->PCA9685_ADDRESS);
    wiringPiI2CWriteReg8(this->fd, this->PCA9685_MODE1, 0x00);
    int prescale = 25000000;
    prescale /= 4096;
    prescale /= 50;
    prescale -= 1;
    int oldmode = wiringPiI2CReadReg8(this->fd, this->PCA9685_MODE1);
    int newmode = (oldmode & 0x7F) | 0x10;
    wiringPiI2CWriteReg8(this->fd, this->PCA9685_MODE1, newmode);
    wiringPiI2CWriteReg8(this->fd, this->PCA9685_PRESCALE, prescale);
    wiringPiI2CWriteReg8(this->fd, this->PCA9685_MODE1, oldmode);
    wiringPiI2CWriteReg8(this->fd, this->PCA9685_MODE1, oldmode | 0x80);    
}

void PCA9685::setServo(int servo, int angle){
    int on = 0;
    int off = angle * 4096 / 180;
    wiringPiI2CWriteReg8(this->fd, LED0_ON_L + 4 * servo, on & 0xFF);
    wiringPiI2CWriteReg8(this->fd, LED0_ON_H + 4 * servo, on >> 8);
    wiringPiI2CWriteReg8(this->fd, LED0_OFF_L + 4 * servo, off & 0xFF);
    wiringPiI2CWriteReg8(this->fd, LED0_OFF_H + 4 * servo, off >> 8);    
}