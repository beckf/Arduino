/*
 * This will control a servo using an IR Decoder.
 * The IR Decoder will be on digital pin 2
 *  For photo see here:
 *  http://labs.da.org/~fbeck/robotics/examples/ir_sensor.jpg
 */

// Include IR Sensor library
#include "IRLibAll.h"

//Create a receiver object to listen on pin 2
IRrecvPCI myReceiver(2);

//Create a decoder object 
IRdecode myDecoder;   

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}

void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }
}
