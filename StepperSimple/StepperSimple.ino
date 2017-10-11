/* 
 * This is a working example of using a stepper motor with the
 * Adafruit Motor Sheild. For more information see below:
 * 
 * https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);
  Serial.println("Stepper test!");
  
  // create with the default frequency 1.6KHz
  AFMS.begin();  
  // 10 rpm
  myMotor->setSpeed(10);     
}

void loop() {
  Serial.println("Single coil steps");
  myMotor->step(100, FORWARD, SINGLE); 
  myMotor->step(100, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  myMotor->step(100, FORWARD, DOUBLE); 
  myMotor->step(100, BACKWARD, DOUBLE);
  
  Serial.println("Interleave coil steps");
  myMotor->step(100, FORWARD, INTERLEAVE); 
  myMotor->step(100, BACKWARD, INTERLEAVE); 
  
  Serial.println("Microstep steps");
  myMotor->step(50, FORWARD, MICROSTEP); 
  myMotor->step(50, BACKWARD, MICROSTEP);
}
