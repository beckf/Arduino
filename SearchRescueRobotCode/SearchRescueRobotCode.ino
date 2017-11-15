#include <Servo.h>

const int trigPin = 5;
const int echoPin = 6;

int rightReading, leftReading;
int rightProxPIN = 3;
int leftProxPIN = 2;
int rightWheelPIN = 9;
int leftWheelPIN = 10;
int debugSerial = 1;

long frontDuration;
int frontDistance;

Servo left;
Servo right;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rightProxPIN, INPUT);
  pinMode(leftProxPIN, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  left.attach(leftWheelPIN);
  right.attach(rightWheelPIN);
  
  Serial.println("Pausing 5 seconds for calibration.");
  left.write(90);
  right.write(90);
  delay(5000);
  
}

void loop() {
  //Ultrasonic Readings
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  frontDuration = pulseIn(echoPin, HIGH);
  frontDistance= frontDuration*0.034/2;
  //Side Readings.
  rightReading = digitalRead(rightProxPIN);
  leftReading = digitalRead(leftProxPIN);

  // Output Readings to Serial Monitor
  Serial.print("Left: ");
  Serial.print(leftReading);
  Serial.print(" Right: ");
  Serial.print(rightReading);
  Serial.print(" Front: ");
  Serial.println(frontDistance); 

  if (frontDistance <= 3 ) {
    //reverse
    left.write(85);
    right.write(95);
    delay(500);
    rightReading = digitalRead(rightProxPIN);
    if (rightReading == 0) {
      left.write(85);
      right.write(85);
    } else {
      left.write(95);
      right.write(95);
    }
  } else if (rightReading == 0) {
    //Serial.println("go right");
    left.write(85);
    right.write(85);
  } else if (leftReading == 0) {
    //Serial.println("go left");
    left.write(95);
    right.write(95);
  } else {
    //Serial.println("go straight");
    left.write(135);
    right.write(45);
  }

  
}
