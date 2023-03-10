#include <iostream>
#include <string>
#include <sstream>
#include <wiringPiI2C.h>

#define HMC_ADDRESS                0x68

/**
 * @class HMC5883L
 * @brief This class handles the sensor HMC5883L.
 */
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
    int getX() const;
    int getY() const;
    int getZ() const;
};
