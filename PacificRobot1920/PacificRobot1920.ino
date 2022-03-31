/* 
 *  Controls a servo using an IR Sensor.
 *  IR Sensor is on Digital PIN 2
 *  For photo see here:
 *  http://labs.da.org/~fbeck/robotics/examples/ir_sensor.jpg
 *  
 * Sensor codes below are setup for black IR remotes.
 */

// LED
const int ledPin =  LED_BUILTIN;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

// Include IR Sensor library
#include <IRremote.h>

// Include the servo library.
#include <Servo.h>

// Constants
// Apple Remote Values in 32bit.
const unsigned long up = 2011254902;
const unsigned long down = 2011246710;
const unsigned long left = 2011271286;
const unsigned long right = 2011258998;
const unsigned long center = 2011275382;
const unsigned long repeat = 4294967295;

//Create a receiver object to listen on pin 2
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// create servo object to control a servo
Servo servoL;
Servo servoR;  

// Variables
int pos;

void setup() {
  // Start the Serial Monitor so we can output debugging test to it.
  Serial.begin(9600);

  // Enable the IR library.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");\
  Serial.println(F("Ready to receive IR signals"));
  
  // attaches the servo on pin 9 to the servo object
  servoR.attach(9);
  servoL.attach(10);
  
  // LED
  pinMode(ledPin, OUTPUT);
  
}

void checkLED() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void loop() {
  
  // Does the LED need to flash?
  checkLED();

  unsigned long current = 0;
  if (irrecv.decode(&results)) {
    unsigned long current = results.value;
    Serial.println(current);
    
    if (current != repeat) {
      
      if (current == left) {
        Serial.println("Left Button");
        servoL.write(100);
        servoR.write(100);
        
      } else if (current == right) {
        Serial.println("Right Button");
        servoL.write(80);
        servoR.write(80);
        
      } else if (current == up) {
        Serial.println("Up Button");
        servoL.write(170);
        servoR.write(10);
        
      } else if (current == down) {
        Serial.println("Down Button");
        servoL.write(60);
        servoR.write(120);
        
      } else if (current == center) {
        Serial.println("Center Button");
        servoL.write(90);
        servoR.write(90);
        
      } else {
        Serial.println("Nothing Matched");
      } 
      
    }

    irrecv.resume();
    
  }
}
