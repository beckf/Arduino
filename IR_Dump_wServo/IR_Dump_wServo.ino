/* 
 *  Controls a servo using an IR Sensor.
 *  IR Sensor is on Digital PIN 2
 *  For photo see here:
 *  http://labs.da.org/~fbeck/robotics/examples/ir_sensor.jpg
 *  
 * Sensor codes below are setup for black IR remotes.
 */

// Include IR Sensor library
#include <IRremote.h>

// Include the servo library.
#include <Servo.h>

// Constants
// Apple Remote Values in 32bit.
const unsigned long up = 2011254929;
const unsigned long down = 2011246737;
const unsigned long left = 2011271313;
const unsigned long right = 2011259025;
const unsigned long center = 2011249297;
const unsigned long play = 2011298449;
const unsigned long menu = 2011283601;

//Create a receiver object to listen on pin 2
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// create servo object to control a servo
Servo servo1;  

// Variables
int pos;

void setup() {
  // Start the Serial Monitor so we can output debugging test to it.
  Serial.begin(9600);

  // Enable the IR library.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");\
  Serial.println(F("Ready to receive IR signals"));
  
  // attaches the servo on pin 9 to the servo object
  servo1.attach(9);
  
}

void loop() {
  //Continue looping until you get a complete signal received
  unsigned long current = 0;
  if (irrecv.decode(&results)) {
    unsigned long current = results.value;
    Serial.println(current);
    switch(current) {
      case left:    pos=170; break;
      case right:   pos=10; break;
      case center:  pos=90; break;
    }
    
      servo1.write(pos); // tell servo to go to position in variable 'pos' 
    // Prepare for next value
    irrecv.resume(); // Receive the next value  
  }
}
