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
int frontIRWallMax = 35;
int rearIRWallMax = 35;
// Min distance that is considered too far from the wall
int frontIRWallMin = 17; 
int rearIRWallMin = 17;

int frontUSWallMin = 3;

int readAnalogSmooth(int pin) {
    // How many readings to take 
    int numReadings = 8;
    int total = 0;
    int average = 0;

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      // convert volts into centimeters
      float volts = analogRead(pin)*0.0048828125;
      float distance = 65*pow(volts, -1.10);
      total = total + distance;
    }
    // Return an average of all the readings in CM
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
  frontDistance = frontDuration*0.034/2;

  frontIRReading = readAnalogSmooth(frontProxPIN);
  rearIRReading = readAnalogSmooth(rearProxPIN);
}

void calibrateSensors() {
  lcd.setCursor(0, 0);
  lcd.print("CalibrateSensors");

  int numReadings = 3;
  
  int frontUCTotal, frontIRTotal, rearIRTotal = 0;
  
  for (int readingCount = 0; readingCount < numReadings; readingCount++) {
      readSensors();
      
      frontIRTotal = frontIRTotal + readAnalogSmooth(frontProxPIN);
      rearIRTotal = rearIRTotal + readAnalogSmooth(rearProxPIN);

      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("U");
      lcd.print(frontIRReading);
      lcd.print(" R");
      lcd.print(rearIRReading);
      delay(1000);
   }

    frontIRWallMin = frontIRTotal/numReadings; 
    rearIRWallMin = rearIRTotal/numReadings;
    frontIRWallMin = frontIRWallMin - 2;
    rearIRWallMin = rearIRWallMin - 2;
    frontIRWallMax = frontIRWallMin + 15;
    rearIRWallMax = rearIRWallMin + 15;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Complete");
    lcd.setCursor(0, 1);
    lcd.print("Front IR:");
    lcd.print(frontIRWallMin);
    lcd.print(" - ");
    lcd.print(frontIRWallMax);
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Rear IR:");
    lcd.print(rearIRWallMin);
    lcd.print(" - ");
    lcd.print(rearIRWallMax);
    delay(2000);
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

  // Calibrate Servos
  lcd.setCursor(0, 0);
  lcd.print("Calibrate Servos");
  left.write(90);
  right.write(90);
  for (int second = 0; second < 6; second++) {
      lcd.setCursor(0, 1);
      lcd.print(second);
      delay(1000);
   }
  
  // Clear the LCD
  lcd.clear();

  // Calibrate Sensrors
  // calibrateSensors();
  // Clear the LCD
  lcd.clear();
  
  
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
    lcd.print("   Corner:    ->");
    if (debug == 0) {
        left.write(100);
        right.write(50);
    }
    
 // What to do when a wall is found.
 } else if ((frontIRReading <= frontIRWallMax && frontIRReading >= frontIRWallMin) && (rearIRReading <= rearIRWallMax && rearIRReading >= rearIRWallMin)) {
    readSensors();
    while (frontIRReading > rearIRReading) {
      lcd.setCursor(0, 0);
      lcd.print("<- Wall Found   ");
      left.write(100);
      right.write(70);
      readSensors();
    } 
    while (frontIRReading < rearIRReading) {
      lcd.setCursor(0, 0);
      lcd.print("   Wall Found ->");
      left.write(108);
      right.write(78);
      readSensors();
    }
 } else {
    lcd.setCursor(0, 0);
    lcd.print("   Wall?        ");
    left.write(100);
    right.write(70);
 }
}
