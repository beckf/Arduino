 /* 
 *  Controls an RC Car with servos using an IR Sensor.
 *  IR Sensor is on Digital PIN 2
 *  
 *  Setup for two servos that drive like a tank.
 *  
 *  Sensor codes below are setup for black Adafruit IR remotes.
 */

// Include IR Sensor library
#include "IRLibAll.h"

// Include the servo library.
#include <Servo.h>

#define MY_PROTOCOL NEC
#define RIGHT_ARROW 0xfd50af
#define LEFT_ARROW 0xfd10ef
#define UP_ARROW 0xfda05f
#define DOWN_ARROW 0xfdb04f
#define STOP 0xfd609f

uint32_t Previous;

//Create a receiver object to listen on pin 2
IRrecvPCI myReceiver(2);

// create servo object to control the left servo
Servo leftServo;
// create servo object to control the rightservo
Servo rightServo;

//Create a decoder object 
IRdecode myDecoder;

void setup() {
  Serial.begin(9600);
  myReceiver.enableIRIn();
  leftServo.attach(9);
  rightServo.attach(10);
}

void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();
    
    if(myDecoder.protocolNum==MY_PROTOCOL) {
      
         if(myDecoder.value==0xFFFFFFFF)
           myDecoder.value=Previous;
           switch(myDecoder.value) {
              case LEFT_ARROW:    Serial.println("Heard LEFT"); leftServo.write(135); rightServo.write(45); break;
              case RIGHT_ARROW:   Serial.println("Heard RIGHT");leftServo.write(45); rightServo.write(135); break;
              case UP_ARROW:      Serial.println("Heard UP");leftServo.write(0); rightServo.write(180); break;
              case DOWN_ARROW:    Serial.println("Heard DOWN");leftServo.write(180); rightServo.write(0); break;
              case STOP:          Serial.println("Heard STOP");leftServo.write(90); rightServo.write(90); break;
           }
         Previous=myDecoder.value;
     }
    
    myReceiver.enableIRIn();
    
  }
}
