/* 
Include the servo library.  The library is some additional code
found in your libraries folder.  This allows us to interface easily
with other devices 
*/
#include <Servo.h>

// create servo object to control a servo
Servo servo1;  

void setup() {
  // put your setup code here, to run once:
  
  // attaches the servo on pin 10 to the servo object
  servo1.attach(10);
  Serial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  // Send the servo to 0 degrees, 90 degrees, 180 degrees, and back.
  servo1.write(1);
  Serial.println("1 degree");
  delay(1000);
  servo1.write(45);
  Serial.println("45 degree");
  delay(1000);
  servo1.write(90);
  Serial.println("90 degree");
  delay(1000);
  servo1.write(135);
  Serial.println("135 degree");
  delay(1000);
  servo1.write(179);
  Serial.println("179 degree");
  delay(1000);
  servo1.write(135);
  Serial.println("135 degree");
  delay(1000);
  servo1.write(90);
  Serial.println("90 degree");
  delay(1000);
  servo1.write(45);
  Serial.println("45 degree");
  delay(1000);
}
