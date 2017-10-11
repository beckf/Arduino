/* 
 *  This example will show you how to control a stepper motor
 *  using a potentiometer.
 *  https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

// the previous reading from the analog input
int previous = 0;
int val = 0;

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
  val = analogRead(0);
  if (val != previous) {
    
    int speed = map(val, 0, 1023, 0, 100);
    
    Serial.println("Potentiometer Value:");
    Serial.println(val);
  
    //move the motor
    myMotor->step(val - previous, FORWARD, SINGLE); 
    
    // remember the previous value of the sensor
    previous = val;
  }
  
  

}
