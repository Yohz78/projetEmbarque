#include "hmc.h"

HMC5883L::~HMC5883L(){
    //close(this->fd);
}

HMC5883L::HMC5883L(){
    fd = wiringPiI2CSetup(HMC_ADDRESS); // HMC5883L I2C address 0x68
    // Set the configuration register to the default value
    wiringPiI2CWriteReg8(fd, 0x00, 0x70); // 8 average, 15 Hz, normal measurement
}

void HMC5883L::readX(){
    int x_raw = wiringPiI2CReadReg8(this->fd, 0x03);
    if(x_raw & 0x8000)
        x_raw = x_raw - 0x10000;
    this->x=x_raw;
}

void HMC5883L::readY(){
    int y_raw = wiringPiI2CReadReg8(this->fd, 0x05);
    if(y_raw & 0x8000)
        y_raw = y_raw - 0x10000;
    this->y=y_raw;
}

void HMC5883L::readZ(){
    int z_raw = wiringPiI2CReadReg8(this->fd, 0x07);
    if(z_raw & 0x8000)
        z_raw = z_raw - 0x10000;
    this->z=z_raw;
}
int HMC5883L::getX()
{
    return this->x;
}

int HMC5883L::getY()
{
    return this->y;
}

int HMC5883L::getZ()
{
    return this->z;
}
