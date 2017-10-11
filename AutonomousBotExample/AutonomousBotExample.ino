
// Include the servo library.
#include <Servo.h>

// create servo object to control a servo
Servo servo1;

// create servo object to control a servo
Servo servo2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // attaches the servo on pin 9 to the a servo object
  servo1.attach(9);
  // attaches the servo on pin 10 to the another servo object
  servo2.attach(10);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on an analog pin 0 and 1
  int sensor1Value = analogRead(A0);
  int sensor2Value = analogRead(A1);
  
  // print out the value you read:
  Serial.print("Sensor 1: ");
  Serial.println(sensor1Value);
  Serial.print("Sensor 2: ");
  Serial.println(sensor2Value);
  
  // If sensor1 detects something, turn left.
  // If sensor2 detects something, turn right.
  
  // SHARP IR reports about 400 to Analog PIN when object is approx. 2 cm away.
  // If less than 400, we are too close and need to turn.
  
  if (sensor1Value < 400) {
    // left turn
    servo1.write(45);
    servo2.write(45);
  } else if (sensor2Value < 400) {
    // right turn
    servo1.write(135);
    servo2.write(135);
  } else {
    //nothing was detect, so move forward
    servo1.write(180);
    servo2.write(0);
  }

}

