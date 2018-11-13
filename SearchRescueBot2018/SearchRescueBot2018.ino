#include <SharpIR.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Adafruit_VCNL4010.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *left = AFMS.getMotor(4);
Adafruit_DCMotor *right = AFMS.getMotor(3);

// Sharp IR on Front
SharpIR frontSensor( SharpIR::GP2Y0A21YK0F, A0 );

// VCNL Left Sensor
Adafruit_VCNL4010 vcnl;

int leftDistance;
int frontDistance;
int ledInterval = 1000;
unsigned long previousMillis = 0;
const int redLED = 10;
const int greenLED = 11;
const int blueLED = 12;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  vcnl.begin();
  
  left->run(RELEASE);
  right->run(RELEASE);

  // LED Pin Outs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  
  // Wait before starting motors.
  delay(3000);
}

void loop() {
    // Check Sensors
    frontDistance = frontSensor.getDistance();
    leftDistance = vcnl.readProximity();

    // Toggle LEDs
    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis >= ledInterval) {
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, LOW);
    }
    
    if (frontDistance <= 11) {
      
      // What to do if the wall is too close.
      // Need to stop and turn right.
      digitalWrite(redLED, HIGH);
      left->run(RELEASE);
      right->run(RELEASE);
      delay(500);
      left->run(FORWARD);
      left->setSpeed(150);
      right->run(BACKWARD);
      right->setSpeed(150);
      while (frontDistance < 18) {
        // Wait here until we move away from wall in front.
        // Prevents rest of loop from running.

        // Check to see if wall is still close
        frontDistance = frontSensor.getDistance();
        delay(500);
      }

    } else if (frontDistance < 18) {
      digitalWrite(greenLED, HIGH);
      // If Front Sensor is within 18cm
      // Make gradual turn right.
      left->run(FORWARD);
      left->setSpeed(255);
      right->run(FORWARD);
      right->setSpeed(50);
      // Check to see if wall is still close
      frontDistance = frontSensor.getDistance();

    } else if ( (leftDistance > 2300 ) && (leftDistance < 3300) ) {
      
        // Left wall out of range. 
        // Gradually turn into wall.
        left->run(FORWARD);
        left->setSpeed(100);
        right->run(FORWARD);
        right->setSpeed(200);
        
    } else if (leftDistance > 4400) {
      
        // Left wall is too close. 
        // Gradually turn away from wall.
        left->run(FORWARD);
        left->setSpeed(200);
        right->run(FORWARD);
        right->setSpeed(100);
        
    } else if (leftDistance < 2300) {
        digitalWrite(blueLED, HIGH);
        // Left wall is missing.  
        // Make big left turn to find it.
        left->run(FORWARD);
        left->setSpeed(150);
        right->run(FORWARD);
        right->setSpeed(200);
        
    } else {
      
        left->run(FORWARD);
        left->setSpeed(200);
        right->run(FORWARD);
        right->setSpeed(200);
        
    }
  
}
  
