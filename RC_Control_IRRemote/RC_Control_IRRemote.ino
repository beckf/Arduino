/*
 * Classroom Example of Servo IR Remote Control
 * 
 */

// Library For IR Remote
#include <IRremote.h>
// Library for Servo
#include <Servo.h>

// Arduino Digital PIN connected to IR Sensor
// These are constants and cannot be changed later in code.
const int IR = 2;

// Arduino PWM Digital PIN Connected to Servo 1
// These are constants and cannot be changed later in code.
const int servo1Pin = 9;
const int servo2Pin = 10;

// Save the reading of the IR here
// This will later be set to a value in the loop
unsigned long int reading;

// Create an IR object as documented for the IRremote library
// https://github.com/z3t0/Arduino-IRremote
IRrecv irrecv(IR);
decode_results result;

// Create two servo objects that we will later manipulate.
Servo servo1;
Servo servo2;

void setup()
{
  // Turn on trhe serial monitor
  Serial.begin(9600);

  // Attach the servos to a PIN on the arduino board
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Set the servos to 90 so they should be in the middle or stopped.
  servo1.write(90);
  servo2.write(90);

  // Tell us we are turning on the IR decoder.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the IR receiver
  Serial.println("Enabled IRin");
}

void loop() {

  /*
   * If the IR Sensor reads something, then check the reading and make a descision on what to do
   */
   
  if (irrecv.decode(&result)) {
    // Remember the value that is coming from the sensor
    reading = (result.value);
    // Print the value to the serial monitor
    Serial.println(reading);

    // Move Servos based on reading
    switch(reading) {
              case 16584943:  Serial.println("Heard LEFT"); servo1.write(45); servo2.write(45); break;
              case 16601263:  Serial.println("Heard RIGHT"); servo1.write(135); servo2.write(135); break;
              case 16621663:  Serial.println("Heard UP"); servo1.write(1); servo2.write(179); break;
              case 16625743:  Serial.println("Heard DOWN"); servo1.write(179); servo2.write(1); break;
              case 16605343:  Serial.println("Heard STOP"); servo1.write(90); servo2.write(90); break;
    }

    /*
     * Another way to write the switch statement above using and XOR statement:
     * 
     * if (reading == 16584943) {
     *        do something.....
     * } else if (reading == 16601263) {
     *        do something diffrent....
     * } else {
     *        If nothing matches do this.....
     * }
     * 
     */      

    irrecv.resume(); // Receive the next value
  }

  // Make LED Blink
  // Add in code from example below to make my LED blink.
  // https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay


}
