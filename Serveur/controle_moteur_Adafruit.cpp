#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Créer un objet pour contrôler la carte PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Définir le numéro de canal pour le servo
#define SERVO_CHANNEL 0

void setup() {
  // Initialiser la communication I2C
  Wire.begin();
  pwm.begin();
  
  // Configurer la fréquence PWM à 50 Hz
  pwm.setPWMFreq(50);
}

void loop() {
  // Définir la position de servo à 0 degrés
  pwm.setPWM(SERVO_CHANNEL, 0, 150);
  delay(1000);
  
  // Définir la position de servo à 90 degrés
  pwm.setPWM(SERVO_CHANNEL, 0, 375);
  delay(1000);
  
  // Définir la position de servo à 180 degrés
  pwm.setPWM(SERVO_CHANNEL, 0, 600);
  delay(1000);
}
