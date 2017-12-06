// Reference https://playground.arduino.cc/Main/CountUpDownTimer

#include<CountUpDownTimer.h>

CountUpDownTimer T(DOWN);

void setup()
{
  Serial.begin(9600);
  // Input the time to count from, Format: H,M,S or just seconds ie. 3600 = 1 hour
  T.SetTimer(0,1,0);
  T.StartTimer();
}

void loop()
{
  T.Timer(); // run the timer
  
  if (T.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
    Serial.print(T.ShowHours());
    Serial.print(":");
    Serial.print(T.ShowMinutes());
    Serial.print(":");
    Serial.print(T.ShowSeconds());
    Serial.print(":");
    Serial.print(T.ShowMilliSeconds());
    Serial.print(":");
    Serial.println(T.ShowMicroSeconds());
    // This DOES NOT format the time to 0:0x when seconds is less than 10.
    // if you need to format the time to standard format, use the sprintf() function.
  }

}
