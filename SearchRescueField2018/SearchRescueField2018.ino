// include the libraries!!!
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "CountUpDownTimer.h"
#include <Ultrasonic.h>
#include <Servo.h>

// create new lcd
Adafruit_LiquidCrystal lcd(0);

// End ultrasonic
Ultrasonic ultrasonic1(12, 10);
Servo flagservo;

// Start Stuff
Ultrasonic ultrasonic2(7, 11);

int redLED = 2;
int greenLED = 3;
int blueLED = 4;

const int startBeam = 8;

const int fieldLEDs[] = {2, 3, 4};
const int fieldLEDInterval = 500;
const int fieldLEDG = 2;
const int fieldLEDR = 3;
const int fieldLEDB = 4;
int previousMillis = 0;
int lastLED = 0;
int startGame = 0;

// start new countdown timer object assigned to T
CountUpDownTimer T(DOWN);
char displayOutput[25];

void setup() {
  // put your setup code here, to run once:
  // Serial
  Serial.begin(9600);
  
  // Field LEDs
  for (int i = 0; i < 3; i++) {
      pinMode(fieldLEDs[i], OUTPUT);
      digitalWrite(fieldLEDs[i], LOW);
  }
  digitalWrite(3, HIGH);

  // LCD Stuff
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Ready!");

  // Strating break beam
  pinMode(startBeam, INPUT);

  //Timer Stuff
  T.SetTimer(0,6,0);
  T.StartTimer();
  
  // End stuff
  flagservo.attach(9);
  flagservo.write(90);
}


// Flash the field LEDs
void flashFieldLEDs() {
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= fieldLEDInterval) {
    previousMillis = currentMillis;
    for (int i = 0; i < 3; i++) {
      digitalWrite(fieldLEDs[i], LOW);
    }
    
    switch (lastLED) {
      case 0:
        digitalWrite(fieldLEDs[0], HIGH);
        ++lastLED;
        break;
      case 1:
        digitalWrite(fieldLEDs[1], HIGH);
        ++lastLED;
        break;
      case 2:
        digitalWrite(fieldLEDs[2], HIGH);
        lastLED = 0;
        break;
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
    Serial.println(ultrasonic1.distanceRead());
  if ( (ultrasonic1.distanceRead() < 20) && ( ultrasonic1.distanceRead() > 0) ) {
    // Game Over
    flagservo.write(0);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Winner!");
    delay(60000);
  }

  if ( (ultrasonic2.distanceRead() < 15)  && ( ultrasonic1.distanceRead() > 0) ) {
    startGame = 1;
  }

  if (startGame == 1) {
    // Start the game
    flagservo.write(90);
    flashFieldLEDs();
    // start timer
    T.Timer();
    if (T.TimeHasChanged()) {       
  
        // Say GO!
        lcd.setCursor(0, 0);
        lcd.print("GO!           ");
  
        // Show timer on LCD
        lcd.setCursor(0, 1);
        sprintf(displayOutput, "%i:%i", T.ShowHours(), T.ShowMinutes());
  
        if (T.ShowHours() <= 9) {
          lcd.print("0");
        }
            
        lcd.print(T.ShowHours());
        lcd.print(":");
        
        if (T.ShowMinutes() <= 9) {
          lcd.print("0");
        }
       
        lcd.print(T.ShowMinutes());
        lcd.print(":");
      
        if (T.ShowSeconds() <= 9) {
          lcd.print("0");
        }
    
        lcd.print(T.ShowSeconds());      
    } else {
         // if countdown timer runs out.
         // GAME OVER
        if (T.TimeCheck()) {
          lcd.setCursor(0, 0);
          lcd.print("You Lose!");
        }
  
    }
  }
  
}
