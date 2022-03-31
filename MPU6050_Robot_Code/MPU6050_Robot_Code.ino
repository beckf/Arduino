// Include some libs
#include <MPU6050_tockn.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Ultrasonic.h>

// Create MPU
MPU6050 mpu6050(Wire);

// Create MotorSheild
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Make some motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

// Create an Ultrasonic
Ultrasonic ultrasonic(10, 11);

// Assign a Buzzer to a PIN
int piezoPin = 9;

// Turns to make from starting position.  
// Positive values = counter clockwise
// Negetive values = clockwise
int turnsArray[] = {90, 180, -360};
// Counts in Array inc 0 so num -1
int totalTurns = 2;
int turnCount = 0;

// Set max speed for motors.
int maxSpeed = 250;

// Set max turning speed
int turningSpeed = 100;

// Using integers instead of floats since speed controller will only take whole numbers.
int zAngle;

int leftSpeed = maxSpeed;
int rightSpeed = maxSpeed;
int rightPreviousSpeed = 0;
int leftPreviousSpeed = 0;

int headingAngle;

// Ultrasonic reading variable
int us; 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  AFMS.begin();

  // Notify start up with three chimes
  // Gives user three seconds to stop moving robot before calibration occurs.
  uint8_t c;
  for (c=0; c<3; c++) {
    tone(piezoPin, 1000, 500);
    delay(1000);
  }
  
  mpu6050.calcGyroOffsets(true);

  // Get starting position/heading
  mpu6050.update();
  headingAngle = mpu6050.getAngleZ();

  leftMotor->setSpeed(leftSpeed);
  leftMotor->run(FORWARD);
  rightMotor->setSpeed(rightSpeed);
  rightMotor->run(FORWARD);
  
}

void loop() {

  // Check Ultrasonic to see if a wall is in range.  
  // Decide to make a turn if so.
  us = ultrasonic.distanceRead();
  //Serial.println(us);
  if ( (us > 2) && (us < 40) ) {
    
    // Play tone to tell everyone we are making a turn.
    tone(piezoPin, 1500, 800);
    
    // Slow down
    // Reduce speed in a for loop instead of just stopping the motors.
    uint8_t i;
    for(i=min(leftPreviousSpeed, rightPreviousSpeed); i>1; i--) {
      rightMotor->setSpeed(i);
      leftMotor->setSpeed(i);
      // Serial.println(i);
      delay(1);
    }
    
    // Stop
    rightMotor->setSpeed(0);
    leftMotor->setSpeed(0);
    delay(1000);

    // If there are no more turns, then stop forever.
    if (turnCount > totalTurns) {
      while(true) {
        delay(1000);
      }
    }

    // Get current angle
    mpu6050.update();
    zAngle = mpu6050.getAngleZ();

    // Add new angle to current angle
    int newAngle = zAngle + turnsArray[turnCount];

    // Decide direction to turn.
    if (newAngle > zAngle ) {
      Serial.println("Left Turn");
      // Turn until new angle is reached
      while (newAngle > zAngle) {
        leftMotor->setSpeed(turningSpeed);
        leftMotor->run(BACKWARD);
        rightMotor->setSpeed(turningSpeed);
        rightMotor->run(FORWARD);
        mpu6050.update();
        zAngle = mpu6050.getAngleZ();
      }
    } else {
      Serial.println("Right Turn");
      // Turn until new angle is reached
      while (newAngle < zAngle) {
        leftMotor->setSpeed(turningSpeed);
        leftMotor->run(FORWARD);
        rightMotor->setSpeed(turningSpeed);
        rightMotor->run(BACKWARD);
        mpu6050.update();
        zAngle = mpu6050.getAngleZ();
      }
    }

    // Set direction of motors back to forward.
    rightMotor->run(FORWARD);
    leftMotor->run(FORWARD);

    // Update new angle to current heading
    headingAngle = newAngle;

    // Increment count
    turnCount = turnCount + 1;
  }
  
  // --- Check MPU that the robot is staying on the proper heading ---
  mpu6050.update();
  zAngle = mpu6050.getAngleZ();

  // Remove all print statements when running robot and not debugging.
  // Serial.print("Heading angle ");
  // Serial.print(headingAngle);
  // Serial.print(" Current angle ");
  // Serial.println(zAngle);

  leftSpeed = maxSpeed;
  rightSpeed = maxSpeed;
    
  if (zAngle > headingAngle) {
    // Drifting to the left
    // Slow down the right wheel based on how far off it is.
    int skewAmount = zAngle - headingAngle;
    rightSpeed = maxSpeed - map(skewAmount,0,180,0,maxSpeed);
    Serial.println(rightSpeed);
    leftSpeed = maxSpeed;
  }
  
  if (zAngle < headingAngle) {
    // Drifting to the right
    // Slow down the left wheel
    int skewAmount = headingAngle - zAngle;
    leftSpeed = maxSpeed - map(skewAmount,0,180,0,maxSpeed);
    Serial.println(leftSpeed);
    rightSpeed = maxSpeed;
  }

  // Write new speed to the motors

  // Change speed on motors.
  rightMotor->setSpeed(rightSpeed);
  leftMotor->setSpeed(leftSpeed);
    
  // Remmeber previous speeds so we can use them later
  leftPreviousSpeed = leftSpeed;
  rightPreviousSpeed = rightSpeed;
  
}
