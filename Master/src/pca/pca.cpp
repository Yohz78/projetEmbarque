#include <unistd.h>
#include <math.h>

#include "pca.h"

/*void PCA9685::init(){
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
}*/

/**
 * @brief Construct a new PCA9685::PCA9685 object
 * 
 * @param i2c_bus 
 * @param i2c_address 
 */
PCA9685::PCA9685(uint8_t i2c_bus, uint8_t i2c_address) : i2c_bus(i2c_bus), i2c_address(i2c_address), nom("PCA9685"), deviceI2C(nullptr)
{
    
}

/**
 * @brief Destroy the PCA9685::PCA9685 object
 * 
 */
PCA9685::~PCA9685(){
    this->clean();
}

/**
 * @brief Initialisation function for the PCA9685 card
 * 
 * @return true 
 * @return false 
 */
bool PCA9685::init(){
    //std::cout  << "Initialisation de la carte de controle des moteurs" << std::endl;
    deviceI2C = new i2c(this->getAdresse(), this->getBus());
    if (!deviceI2C->getError()){

        deviceI2C->WriteReg8(MODE2, OUTDRV);
        deviceI2C->WriteReg8(MODE1, ALLCALL);
        //time.sleep(0.005)  # wait for oscillator
        usleep(5000);
        uint8_t mode1 = (uint8_t)deviceI2C->ReadReg8(MODE1);
        mode1 = mode1 & ~SLEEP;  // wake up (reset sleep)
        deviceI2C->WriteReg8(MODE1, mode1);
        //time.sleep(0.005)  # wait for oscillator
        usleep(5000);
        setPwmFrequency(50);

        error = false;
        
    }
    else{
	    error = true;
    }

    return !error;
}

/**
 * @brief CLean function for the destructor
 * 
 */
void PCA9685::clean()
{
    if (deviceI2C != NULL)
    {
        delete deviceI2C;
        deviceI2C = nullptr;
    }
}

/**
 * @brief Function to set pwm
 * 
 * @param frequency 
 */
void PCA9685::setPwmFrequency(uint16_t frequency){
    float prescaleval = 25000000.0; //25MHz
    prescaleval /= 4096.0;         //12-bit
    prescaleval /= float(frequency);
    prescaleval -= 1.0;
    //std::cout <<"Setting PWM frequency to "<<frequency<<" hz" << std::endl;
    //std::cout <<"Estimated pre-scale: "<<prescaleval << std::endl;
    int prescale = (int)floor(prescaleval + 0.5);
    //std::cout <<"Final pre-scale: "<<prescale << std::endl;
    uint8_t oldmode = (uint8_t)deviceI2C->ReadReg8(MODE1);
    uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
    deviceI2C->WriteReg8(MODE1, newmode);      //go to sleep
    deviceI2C->WriteReg8(PRESCALE, prescale);
    deviceI2C->WriteReg8(MODE1, oldmode);
    //time.sleep(0.005)  # wait
    usleep(5000);
    deviceI2C->WriteReg8(MODE1, oldmode | 0x80);
}

/**
 * @brief Function setting pwm 
 * 
 * @param channel 
 * @param on 
 * @param off 
 */
void PCA9685::setPwm(uint8_t channel, uint16_t on, uint16_t off){
    deviceI2C->WriteReg8(LED0_ON_L+4*channel, on & 0xFF);
    deviceI2C->WriteReg8(LED0_ON_H+4*channel, on >> 8);
    deviceI2C->WriteReg8(LED0_OFF_L+4*channel, off & 0xFF);
    deviceI2C->WriteReg8(LED0_OFF_H+4*channel, off >> 8);
}

/**
 * @brief Function setting pwm
 * 
 * @param on 
 * @param off 
 */
void PCA9685::setAllPwm(uint16_t on, uint16_t off){
    deviceI2C->WriteReg8(ALL_LED_ON_L, on & 0xFF);
    deviceI2C->WriteReg8(ALL_LED_ON_H, on >> 8);
    deviceI2C->WriteReg8(ALL_LED_OFF_L, off & 0xFF);
    deviceI2C->WriteReg8(ALL_LED_OFF_H, off >> 8);
}

/**
 * @brief Move the servo motor
 * 
 * @param channel 
 * @param deg 
 */
void PCA9685::move(uint8_t channel, int deg){
    float pwm = 570.0 + ((deg/180.0) * 1700.0);
    pwm = (4096.0/20000.0) * pwm;
    setPwm(channel, 0, (int) pwm);
}

/**
 * @brief Move the servo motor with the Yellow flag
 * 
 * @param deg 
 */
void PCA9685::moveYellowFlag(int deg)
{
    move(yellowFlagChannel, deg);
}

/**
 * @brief Move the servo motor with the Blue flag
 * 
 * @param deg 
 */
void PCA9685::moveBlueFlag(int deg)
{
    move(blueFlagChannel, deg);
}

/**
 * @brief Getter for error
 * 
 * @return true 
 * @return false 
 */
bool PCA9685::getError()
{
	return error;
}

/**
 * @brief Getter for i2c_adress
 * 
 * @return uint8_t 
 */
uint8_t PCA9685::getAdresse()
{
    return i2c_address;
}

/**
 * @brief Getter for i2c_bus
 * 
 * @return uint8_t 
 */
uint8_t PCA9685::getBus()
{
    return i2c_bus;
}