//  Digital Ambient Sensor

/*
 * This sensor returns 5V or HIGH when the threshold is not reached.
 * When sound reaches the threshold, 0V or LOW is returned.
 * The Threshold can be set by using the blue potentiometer on the board.
 */

int a = 0;

void setup() {
  // turn on the serial port
  Serial.begin(9600);
  
  // initialize the digital pin 2 (sound sensor) as an input
  pinMode(3, INPUT);
  
}

void loop() {
  delay(1000);
  a = digitalRead(3);
  
  // print the result to the serial port
  Serial.print("Sensor Reading:");
  Serial.println(a);   
}
