#include "hmc.h"

HMC5883L::~HMC5883L(){
    close(this->fd);
}

HMC5883L::HMC5883L(){
    fd = wiringPiI2CSetup(0x1E); // HMC5883L I2C address
    // Set the configuration register to the default value
    wiringPiI2CWriteReg8(fd, 0x00, 0x70); // 8 average, 15 Hz, normal measurement    
}

void HMC5883L::readX(){
    unsigned char data[6];
    wiringPiI2CReadReg8(fd, 0x03);
    int x = (int)(data[0] << 8 | data[1]);
    this->x=x;
}

void HMC5883L::readY(){
    unsigned char data[6];
    wiringPiI2CReadReg8(fd, 0x03);
    int y = (int)(data[2] << 8 | data[3]);
    this->y=y;
}

void HMC5883L::readZ(){
    unsigned char data[6];
    wiringPiI2CReadReg8(fd, 0x03);
    int z = (int)(data[4] << 8 | data[5]);
    this->z=z;
}

void HMC5883L::readTemp(){
    int temperature = wiringPiI2CReadReg16(fd, 0x41);
    this->temp=temperature;
}