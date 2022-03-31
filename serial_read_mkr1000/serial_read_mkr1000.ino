
#include <SoftwareSerial.h>

SoftwareSerial linkSerial(13, 14);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  linkSerial.begin(4800);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (linkSerial.available()) {
    Serial.println(linkSerial.read());
  }
}
