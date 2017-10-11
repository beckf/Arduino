// Digital Proximity Sensor

/* 
 *  The digital proximity sensor will return HIGH (5V) or LOW (0V) by measuring distance with IR light.
 *  To set the threshold on the IR Sensor, use the blue potentiometer.
 *  This sensor will always return HIGH until the threshold is met switching the out pin to LOW.
 *  
 *  This sensor is different from the SHARP IR Sensor as it only tells you if the threshold was reached.
 *  Use the SHARP IR sensor if you wish to read the actual distance from the IR Sensor (which is analog).
 *  
 *  http://labs.da.org/~fbeck/robotics/examples/digital_ir_sensor.jpg
 */

//  Define the variables a,b, and c as an  integer
//  Digital Proximity Sensor returns a 1 (true) or 0 (false)
int a = 0;


// do some setup.  this code runs once to do any beginning setup you have.
void setup() {
  // turn on the serial port
  Serial.begin(9600);
  // initialize digital pin 13 (LED) as an output.
  pinMode(13, OUTPUT);
  // initialize the digital pin 2 (proximity sensor) as an input
  pinMode(2, INPUT);
  
}

// once started, this code will run continuously. 
void loop() {

    // turn on the arduino led on pin 13 while we are taking a reading
    digitalWrite(13, HIGH);
    // read the sensor
    a = digitalRead(2);
    
    delay(1000);
    //turn off the arduino led on pin 13.
    digitalWrite(13, LOW);
    delay(1000);
  
    // print the result to the serial port
    Serial.print("Sensor Reading:");
    Serial.println(a);
    
}
