// SHARP IR Sensor

/* This Analog sensor will read the amount of IR light that is reflected from a surface.
 * The light reflected back can be used to somewhat measure distance.
 * Many factors can affect this sensors perfomance including ambient light, and light/dark colors. 
 */

//  Define the variable a as an integer

int a = 0;

// do some setup.  this code runs once to do any beginning setup you have.
void setup() {
  // turn on the serial port
  Serial.begin(9600);
  // initialize digital pin 13 (LED) as an output.
  pinMode(13, OUTPUT);
  
}

// once started, this code will run continuously. 
void loop() {

    // turn on the arduino led on pin 13 while we are taking a reading
    digitalWrite(13, HIGH);
    a = analogRead(1);
    delay(1000);
    //turn off the arduino led on pin 13.
    digitalWrite(13, LOW);

    // if the sensor is greater than zero
    if( a>0 ) {
  
    Serial.print("Sensor Reading:");
    Serial.println(a);

    delay(1000);
    
    }
}
