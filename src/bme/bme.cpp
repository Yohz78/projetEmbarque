#pragma once


bme::bme()
{
    fd = wiringPiI2CSetup("adresse du module" );
    wiringPiI2CWriteReg8(fd, 0x00, 0x70);
}

bme::~bme()
{
    close(this->fd);
}

void bme::getTemperature()
{

    this->temperature = temperatureCapteur;
}

void bme::getPression()
{
    this->pression = pressionCapteur;
}

void bme::getHumidite()
{
    // msb is on 0x0FD
    // lsb is on 0x0FE
    int humiditeCapteur;
    int humidite_msb = wiringPiI2CReadReg8(fd, 0xFD);
    int humidite_lsb = wiringPiI2CReadReg8(fd, 0xFE);
    this->humidite = humiditeCapteur;
}
