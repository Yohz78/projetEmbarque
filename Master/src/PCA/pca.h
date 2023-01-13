#include <wiringPiI2C.h>

class PCA9685 {
    private:
        int fd;
        const int PCA9685_ADDRESS = 0x40;
        const int PCA9685_MODE1 = 0x00;
        const int PCA9685_PRESCALE = 0xFE;
        const int SERVO_1 = 0;
        const int SERVO_2 = 1;

    public:
        void init();

        void setServo(int servo, int angle) {
            int on = 0;
            int off = angle * 4096 / 180;
            wiringPiI2CWriteReg8(fd, LED0_ON_L + 4 * servo, on & 0xFF);
            wiringPiI2CWriteReg8(fd, LED0_ON_H + 4 * servo, on >> 8);
            wiringPiI2CWriteReg8(fd, LED0_OFF_L + 4 * servo, off & 0xFF);
            wiringPiI2CWriteReg8(fd, LED0_OFF_H + 4 * servo, off >> 8);
        }
};