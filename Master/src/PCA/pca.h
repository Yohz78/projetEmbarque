#include <wiringPiI2C.h>
#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

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

        void setServo(int servo, int angle);
};