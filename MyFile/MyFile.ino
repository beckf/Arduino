/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
//twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

const int servoPin = 9;

void setup() {
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void stopServo() {
  myservo.write(90);
}

void loop() {


  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  int currentPosition = myservo.read();
  if ( currentPosition > 100 && currentPosition < 180 ) {
    // turn on LED.
    uytuyyuytukjhkkjhkjhkhkhkjhkjhkjhkjhkkh
    kb'b'ouh
  } else if ( currentPosition > 0 && currentPosition < 99 ) {
    // do code
    hgfhgfhfhfhgfhg
  } else {
    // dunno
    Serial.println("error");
  }

  switch (pos) {
  case 1.45:
    // statements
    // move servo, turn LED, read ultrasonic.
    break;
  case 2:
    // statements
    //  more custom code.
    break;
  default:
    // statements
    // 
    break;
  }
  
  stopServo();

  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  
}
