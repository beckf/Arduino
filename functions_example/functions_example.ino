#include <Ultrasonic.h>

Ultrasonic ultrasonic1(12, 13);
Ultrasonic ultrasonic2(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

bool readFlex() {
  // analog A0 is a flex sensor
  // if the read is over 500 then it has bent enough to trigger true
  if ( analogRead(A0) > 500 ) {
    return true;
  } else {
    return false;
  }
}

int closestUltrasonic() {
  int us1 = ultrasonic1.distanceRead();
  int us2 = ultrasonic2.distanceRead();
  if (us1 > us2) {
    return 1;
  } else if (us2 > us1) {
    return 2;
  } else if (us1 == us2) {
    return "same";
  } else {
    return "unknown";
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (readFlex()) {
    // flex returned true, do something.
    servo.write(100);
  } else if (closestUltrasonic() == 1) {
    // move a servo
    servo.write(80);
  } else {
    // all is false
  }

  
  
  
}
