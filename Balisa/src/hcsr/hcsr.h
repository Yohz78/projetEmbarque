#include <iostream>
#include <string>
#include <sstream>
#include <wiringPi.h>

/**
 * @class HCSR50
 * @brief Class to manipulate the HCSR50 sensor
 */
class HCSR50 {
  private:
    int motionPin; // Port GPIO de la balise
    bool motionDetected; // Booleen refletant la présence (True or False)

  public:
    // Constructor
    HCSR50();
    // Destructor
    ~HCSR50();

    // Function to check if motion was detected and return data as string
    std::string checkMotion();
    std::string watcherMotion();

};
