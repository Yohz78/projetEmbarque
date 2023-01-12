#include <iostream>
#include <string>
#include <sstream>
#include <wiringPi.h>

class HCSR50 {
  private:
    int motionPin;
    bool motionDetected;

  public:
    // Constructor
    HCSR50();
    //
    ~HCSR50();

    // Function to check if motion was detected
    bool checkMotion();

};