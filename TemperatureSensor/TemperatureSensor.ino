const int analogPIN = 0;

void setup() {
  Serial.begin(9600);
  pinMode(analogPIN, INPUT);
  
}

void loop() {
  float temp = analogRead(analogPIN);
  // conver to fahrenheit
  float tempInF = temp * 0.004882814 * 100;
  Serial.println(tempInF);
  delay(1000);
}
