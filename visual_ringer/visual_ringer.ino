#include <Adafruit_INA260.h>
Adafruit_INA260 ina260 = Adafruit_INA260();

int red = 9;
int flashIntervals = 5;
int flashSpeed = 50;
int ringCount = 5;
float inputVoltage;

void setup() {
  // put your setup code here, to run once:
  pinMode(red, OUTPUT);
  pinMode(A5, INPUT_PULLUP);
  Serial.begin(9600);

  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");
  
}

void flash() {
  int count = 0;
  while (count <= flashIntervals) {
    digitalWrite(red, HIGH);
    delay(flashSpeed);
    digitalWrite(red, LOW);
    delay(flashSpeed);
    count = count + 1;
  }
}

void loop() {
  int a = analogRead(5);
  if ( a <= 859 ) {
    Serial.println("Ring");
    flash();
  }
  delay(10);
}
