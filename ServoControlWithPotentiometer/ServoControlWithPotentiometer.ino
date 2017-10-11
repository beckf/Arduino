/* 
Include the servo library.  The library is some additional code
found in your libraries folder.  This allows us to interface easily
with other devices 
*/
#include <Servo.h>

// create servo object to control a servo
Servo servo1;  

// analog pin used to connect the potentiometer
int potpin = 0;
// variable to read the value from the analog pin
int val;
 
void setup() {
  servo1.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {
  
  // reads the value of the potentiometer (value between 0 and 1023)
  val = analogRead(potpin);
  Serial.println(val);
  // scale it to use it with the servo (value between 0 and 180)
  val = map(val, 0, 1023, 0, 180);
  // sets the servo position according to the scaled value
  servo1.write(val);
  // waits for the servo to get there
  delay(15);
}

