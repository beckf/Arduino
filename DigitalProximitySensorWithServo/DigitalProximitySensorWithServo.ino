/* 
 *  This will reverse the direction of a servo if the proximity sensor detects a barrier.
 *  http://labs.da.org/~fbeck/robotics/examples/digital_ir_sensor_servo.mov
 */

/* 
Include the servo library.  The library is some additional code
found in your libraries folder.  This allows us to interface easily
with other devices 
*/
#include <Servo.h>

// create servo object to control a servo
Servo servo1; 

int a = 0;

// do some setup.  this code runs once to do any beginning setup you have.
void setup() {
  // turn on the serial port
  Serial.begin(9600);
  // initialize digital pin 13 (LED) as an output.
  pinMode(13, OUTPUT);
  // initialize the digital pin 2 (proximity sensor) as an input
  pinMode(2, INPUT);
  // attaches the servo on pin 9 to the servo object
  servo1.attach(9);
  
}

// once started, this code will run continuously. 
void loop() {
    a = digitalRead(2);
    Serial.println(a);
    
    if(a == 1) {
      servo1.write(179);
    } else {
      servo1.write(1);
    }
    
}
