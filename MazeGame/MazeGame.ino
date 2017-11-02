// include the libraries!
#include "Wire.h"
#include "Adafruit_VCNL4010.h"
#include "Adafruit_LiquidCrystal.h"
#include "CountUpDownTimer.h"

// map buzzer notes to actual frequencies
#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816
#define  d4    3401 
#define  e4    3030 
#define  f4    2865
#define  g4    2551
#define  a4    2272 
#define  a4s   2146
#define  b4    2028 
#define  c5    1912
#define  d5    1706
#define  d5s   1608
#define  e5    1517
#define  f5    1433
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
// define buzzer rest note
#define  R     0

// create new lcd
Adafruit_LiquidCrystal lcd(3, 2, 4);

// create new vcnl for starter proximity sensor
Adafruit_VCNL4010 vcnl;

// establish some constatnts
const int readyLED = 13;
const int finishLED1 = 9;
const int finishLED2 = 10;
const int goLED = 12;
const int buzzerPIN = 8;
const int finishProx = A0;
const int finishThreshold = 500;
const int startThreshold = 2500;

const int fieldLEDs[] = {5, 6, 7};
const int fieldLEDInterval = 200;
const int fieldLEDG = 5;
const int fieldLEDR = 6;
const int fieldLEDB = 7;

// other vars
int finishReading = 0;
int startProx = 1;
int botReady = 0;
int previousMillis = 0;
int lastLED = 0;

//buzzer vars
int toneM = 0;
int beat = 0;
long duration  = 0;
int rest_count = 50;

// Melody 1: Star Wars Imperial March
int melody1[] = {  a4, R,  a4, R,  a4, R,  f4, R, c5, R,  a4, R,  f4, R, c5, R, a4, R,  e5, R,  e5, R,  e5, R,  f5, R, c5, R,  g5, R,  f5, R,  c5, R, a4, R};
int beats1[]  = {  50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5,  20, 5, 60, 40};
 
// Melody 2: Star Wars Theme
int melody2[] = {  f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s,   c5, f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s,   c5};
int beats2[]  = {  21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128, 21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128}; 

int MAX_COUNT = sizeof(melody1) / 2;

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;

// start new countdown timer object assigned to T
CountUpDownTimer T(DOWN);
char displayOutput[25];

void setup() {
  
  //PIN Stuff
  pinMode(readyLED, OUTPUT);
  pinMode(goLED, OUTPUT);
  pinMode(finishLED1, OUTPUT);
  pinMode(finishLED2, OUTPUT);

  for (int i = 0; i < 3; i++) {
      pinMode(fieldLEDs[i], OUTPUT);
      digitalWrite(fieldLEDs[i], LOW);
  }
  
  // LCD Stuff
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);

  //Serial Stuff
  Serial.begin(9600);

  //Timer Stuff
  T.SetTimer(0,6,0);
  T.StartTimer();

  //Buzzer Stuff
  pinMode(buzzerPIN, OUTPUT);

  // Wait for starting proximity sensor to be found
  if (! vcnl.begin()){
    Serial.println("Sensor not found!");
    while (1);
  }
  
}

// PLAY TONE
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (toneM > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
 
      digitalWrite(buzzerPIN,HIGH);
      delayMicroseconds(toneM / 2);
 
      // DOWN
      digitalWrite(buzzerPIN, LOW);
      delayMicroseconds(toneM / 2);
 
      // Keep track of how long we pulsed
      elapsed_time += (toneM);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
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

  //Serial.println(vcnl.readProximity());
  //Serial.println(startProx);
  
  // check the finish proximity sensor for a nearby bot.  
  // only change the value if less than threshold.
  if (finishReading <= finishThreshold) {
    finishReading = analogRead(finishProx);
  } 

  // if a bot is detected at finish line then end the game
  if (finishReading >= finishThreshold) {
    
    // write to lcd
    lcd.setCursor(0, 0);
    lcd.print("Life Saved. Win!");
    
    //Flash Finish LEDs
    digitalWrite(finishLED1, HIGH);
    digitalWrite(finishLED2, HIGH);

    // Turn on all field LEDs
    for (int i = 0; i < 3; i++) {
      digitalWrite(fieldLEDs[i], HIGH);
    }
    
    // playback the victory music
    for (int i=0; i<MAX_COUNT; i++) {
      toneM = melody2[i];
      beat = beats2[i];
      
      duration = beat * tempo; // Set up timing
 
      playTone(); // A pause between notes
      delayMicroseconds(pause);
    }

  // if the game is not over  
  } else {

      // check for a start of game.
      // only check if the threshold is met
      // botReady allows player to place bot without tripping the sensor.
      if (botReady == 0) {
        startProx = vcnl.readProximity();
        digitalWrite(fieldLEDR, HIGH);
        digitalWrite(fieldLEDG, LOW);
        digitalWrite(fieldLEDB, LOW);
        
        if (startProx >= startThreshold) {
          botReady = 1;
          digitalWrite(fieldLEDG, HIGH);
          digitalWrite(fieldLEDR, LOW);
          digitalWrite(fieldLEDB, LOW);
        }
      } else if ((botReady == 1) && (startProx >= startThreshold)) {
        startProx = vcnl.readProximity();
      }

      // if the prox detected a game start
      if ((startProx <= startThreshold) && (botReady == 1)) {
        
        //Set ready LED off
        digitalWrite(readyLED, LOW);
    
        //Set go LED on
        digitalWrite(goLED, HIGH);

        // Flash the field LEDs
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

              // playback game over music
              // Set up a counter to pull from melody1[] and beats1[]
              for (int i=0; i<MAX_COUNT; i++) {
                toneM = melody1[i];
                beat = beats1[i];
 
                duration = beat * tempo; // Set up timing
 
                playTone(); // A pause between notes
                delayMicroseconds(pause);
              }


            }
         }
         
      // if game hasn't started, then tell player we are ready.
      } else if (botReady == 1) {  
        lcd.setCursor(0, 0);
        lcd.print("Bot Loaded!   ");
        //Set ready LED on
        digitalWrite(readyLED, HIGH);
        //Set go LED off
        digitalWrite(goLED, LOW);
      } else {
          //Set ready LED on
          digitalWrite(readyLED, HIGH);
          //Set go LED off
          digitalWrite(goLED, LOW);
          
          lcd.setCursor(0, 0);
          lcd.print("Ready to play!");
      }
  }
    
}
