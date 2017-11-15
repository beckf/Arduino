#include <Servo.h>
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

const int trigPin = 5;
const int echoPin = 6;

const int lcdSensorsInterval = 1000;
unsigned long lcdPreviousMillis = 0;

int frontIRReading, rearIRReading;
int frontProxPIN = A1;
int rearProxPIN = A2;

int rightWheelPIN = 9;
int leftWheelPIN = 10;

long frontDuration;
int frontDistance;

// if debug = 1, then do not move servos.
int debug = 0;

Adafruit_LiquidCrystal lcd(0);
Servo left;
Servo right;

// Max distance that is considered still close to wall
int frontIRWallMax = 700;
int rearIRWallMax = 700;
// Min distance that is considered too far from the wall
int frontIRWallMin = 500; 
int rearIRWallMin = 500;

int frontUSWallMin = 4;

int readAnalogSmooth(int pin) {
    
    int numReadings = 10;
    int total = 0;
    int average = 0;
    
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      total = total + analogRead(pin);
    }
    
    return total / numReadings;
}

void readSensors() {
  //Ultrasonic Readings
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  frontDuration = pulseIn(echoPin, HIGH);
  frontDistance= frontDuration*0.034/2;

  //rearLeftReading = digitalRead(rearLeftProxPIN);
  frontIRReading = readAnalogSmooth(frontProxPIN);
  rearIRReading = readAnalogSmooth(rearProxPIN);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  left.attach(leftWheelPIN);
  right.attach(rightWheelPIN);

  lcd.setCursor(0, 0);
  lcd.print("Calibration");
  left.write(90);
  right.write(90);
  delay(5000);
  
}

void loop() {

  readSensors();
  unsigned long currentMillis = millis();
  
  // Output Readings to LCD
  if (currentMillis - lcdPreviousMillis >= lcdSensorsInterval) {
    lcdPreviousMillis = currentMillis;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("U");
      lcd.print(frontIRReading);
      lcd.print(" R");
      lcd.print(rearIRReading);
      lcd.print(" F");
      lcd.print(frontDistance); 
  }
  

  
  // Slow Right Turn L95 R95
  // Slow Left Trun L85 R85
  // Straight L100 R78

// What to do in a corner
 if ((frontDistance <= frontUSWallMin) && (frontIRReading <= frontIRWallMax && frontIRReading >= frontIRWallMin) && (rearIRReading <= rearIRWallMax && rearIRReading >= rearIRWallMin) ) {
    lcd.setCursor(0, 0);
    lcd.print("Corner- Go Right");
    left.write(90);
    right.write(90);
    delay(250);
    
    readSensors();
    while ( !(frontIRReading <= frontIRWallMax && frontIRReading >= frontIRWallMin) && !(rearIRReading <= rearIRWallMax && rearIRReading >= rearIRWallMin) ) {
      if (debug == 0) {
        left.write(95);
        right.write(95);
      }
      readSensors();
    }
    
 // What to do when a wall is found.
 } else if ((frontIRReading <= frontIRWallMax && frontIRReading >= frontIRWallMin) && (rearIRReading <= rearIRWallMax && rearIRReading >= rearIRWallMin)) {
    lcd.setCursor(0, 0);
    lcd.print("Wall Found      ");
    readSensors();
    if (frontIRReading > rearIRReading) {
      float rate = frontIRReading / rearIRReading;
      int degree = rate * 5;
      left.write(max(100 - degree, 90));
      right.write(78);
    } else {
      float rate = rearIRReading / frontIRReading;
      int degree = rate * 5;
      left.write(100);
      right.write(min(78 + degree, 90));
    }
 } else {
    left.write(90);
    right.write(90);
    lcd.setCursor(0, 0);
    lcd.print("No Wall        ");
 }
}
