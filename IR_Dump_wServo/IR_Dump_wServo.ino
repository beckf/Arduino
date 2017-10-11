/* 
 *  Controls a servo using an IR Sensor.
 *  IR Sensor is on Digital PIN 2
 *  For photo see here:
 *  http://labs.da.org/~fbeck/robotics/examples/ir_sensor.jpg
 *  
 * Sensor codes below are setup for black IR remotes.
 */

// Include IR Sensor library
#include "IRLibAll.h"

// Include the servo library.
#include <Servo.h>

#define MY_PROTOCOL NEC
#define RIGHT_ARROW 0xfd50af
#define LEFT_ARROW 0xfd10ed
#define STOP 0xfd609f

//Create a receiver object to listen on pin 2
IRrecvPCI myReceiver(2);

// create servo object to control a servo
Servo servo1;

//Create a decoder object 
IRdecode myDecoder;   

int16_t pos;         // variable to store the servo position 
int16_t Speed;       // Number of degrees to move each time a left/right button is pressed
uint32_t Previous;   //handles NEC repeat codes
int a = 0;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
  // attaches the servo on pin 10 to the servo object
  servo1.attach(9);
  // initialize the digital pin 2 (proximity sensor) as an input
  pinMode(3, INPUT);
  pos = 90;
  Speed = 3;
}

void loop() {
  delay(1000);
  a = digitalRead(3);
  Serial.println(a);
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail

    if(myDecoder.protocolNum==MY_PROTOCOL) {
         if(myDecoder.value==0xFFFFFFFF)
           myDecoder.value=Previous;
         switch(myDecoder.value) {
            case LEFT_ARROW:    pos=min(180,pos+Speed); break;
            case RIGHT_ARROW:   pos=max(0,pos-Speed); break;
            case STOP:          pos=90; break;
         }
         servo1.write(pos); // tell servo to go to position in variable 'pos' 
         Previous=myDecoder.value;
       }
    
    myReceiver.enableIRIn();      //Restart receiver
    
  }
}
