#include <Ultrasonic.h>
#include <Servo.h>
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

int frontIRReading, rearIRReading;
int frontProxPIN = A1;
int rearProxPIN = A2;

int rightWheelPIN = 9;
int leftWheelPIN = 10;

long frontDuration;
int frontDistance;

// Blinky LED without any delay
const int ledPin =  LED_BUILTIN;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

// if debug = 1, then do not move servos.
int debug = 0;

Adafruit_LiquidCrystal lcd(0);
Servo left;
Servo right;
Ultrasonic ultrasonic(5, 6, 30000UL);

// Create a wall buffer to stay in
// Max distance that is considered still close to wall
int frontIRWallMax = 35;
int rearIRWallMax = 35;
// Min distance that is considered too far from the wall
int frontIRWallMin = 17; 
int rearIRWallMin = 17;
// Distance in front in CM.
int frontUSWallMin = 5;

// Read the analog sensors several times and return an average.
// Helps smooth out irregular in readings.
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

// Read all the sensors.
void readSensors() {
  //Ultrasonic Readings
  frontDistance = ultrasonic.distanceRead(CM);
  // IRs
  frontIRReading = readAnalogSmooth(frontProxPIN);
  rearIRReading = readAnalogSmooth(rearProxPIN);
}

// Function to allow the bot to take several readings from the IRs to 
// determine what is considered close to the wall.
// To use, place bot on left wall and call function in setup()
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
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  
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

  lcd.clear();

}

void loop() {

  // Blinky LED
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
  }

  // Delay the entire loop to prevent sensors from overpowering
  delay(50);

  // Read Sensors
  readSensors();
  
  // Output Readings to LCD
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("U");
  lcd.print(frontIRReading);
  lcd.print(" R");
  lcd.print(rearIRReading);
  lcd.print(" F");
  lcd.print(frontDistance); 

  // Servo Directions and Speeds:
  // Slow Right Turn L95 R95
  // Slow Left Trun L85 R85
  // Straight L100 R78

// What to do in a corner
 if ((frontDistance <= frontUSWallMin) && (frontDistance > 0)) {
    lcd.setCursor(0, 0);
    lcd.print("   Corner:    ->");
    if (debug == 0) {
        // Backup
        left.write(80);
        right.write(102);
        delay(250);

        // Turn Right
        left.write(95);
        right.write(95);
        delay(250);
    }
    
 // What to do when a wall is found.
 } else if ((frontIRReading <= frontIRWallMax && frontIRReading >= frontIRWallMin) && (rearIRReading <= rearIRWallMax && rearIRReading >= rearIRWallMin)) {
    readSensors();
    while (frontIRReading > rearIRReading) {
      // Veer to the left
      lcd.setCursor(0, 0);
      lcd.print("<- Wall Found   ");
      left.write(100);
      right.write(70);
      readSensors();
    } 
    while (frontIRReading < rearIRReading) {
      // Veer to the right
      lcd.setCursor(0, 0);
      lcd.print("   Wall Found ->");
      left.write(108);
      right.write(78);
      readSensors();
    }
 } else {
    // Make slow left circle till wall is found.
    lcd.setCursor(0, 0);
    lcd.print("   Wall?        ");
    left.write(100);
    right.write(70);
 }
}
