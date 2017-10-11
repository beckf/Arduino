#include <Wire.h>
#include "Adafruit_VCNL4010.h"
#include "Adafruit_LiquidCrystal.h"

Adafruit_VCNL4010 vcnl;
Adafruit_LiquidCrystal lcd(3, 2, 4);

long amb,prox;

void setup() {
  Serial.begin(9600);
  vcnl.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("Proximity Test");

}

void loop() {
   lcd.clear();
   
   amb = vcnl.readAmbient();
   prox = vcnl.readProximity();

   Serial.println(String("Ambient: ") + amb);
   Serial.println(String("Proximity: ") + prox);
   
   lcd.setCursor(0, 0);
   lcd.print(String("Amb: ") + String(amb)); 
   
   lcd.setCursor(0, 1);
   lcd.print(String("Prox: ") + String(prox));
   
   delay(1000);
}
