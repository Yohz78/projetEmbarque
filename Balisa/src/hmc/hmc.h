#include <iostream>
#include <string>
#include <sstream>
#include <wiringPiI2C.h>

class HMC5883L{
  private:
    int fd; // I2C file descriptor
    int x, y, z, temp;

  public:
    // Constructor
    HMC5883L();

    // Destructor
    ~HMC5883L();

    // Read data from the sensor
    void readX();
    void readY();
    void readZ();
    int getX();
    int getY();
    int getZ();
};
