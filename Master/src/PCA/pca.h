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

        void setServo(int servo, int angle);
};