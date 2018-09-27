#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
const int UPBUTTON = 2;
const int DOWNBUTTON = 3;
const int PUMP = 12;
int UPBUTTONSTATE = 0;
int DOWNBUTTONSTATE = 0;
int WATERAMOUNT = 1900;

void setup() {
  pinMode(PUMP, OUTPUT); 
  pinMode(UPBUTTON, INPUT);
  pinMode(DOWNBUTTON, INPUT);
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();
}

void loop() {
  digitalWrite(PUMP, HIGH);
  DateTime now = rtc.now();
  UPBUTTONSTATE = digitalRead(UPBUTTON);
  DOWNBUTTONSTATE = digitalRead(DOWNBUTTON);
  
  Serial.println(WATERAMOUNT);
  
  if (UPBUTTONSTATE == 1) {
    moreWater();
  }
  else if (DOWNBUTTONSTATE == 1) {
    lessWater();
  }
  // 10:10:59
  else if (now.hour() == 10 && now.minute() == 10 && now.second() == 59) {
    water();
  }
  // 3:59:59
  else if (now.hour() == 15 && now.minute() == 59 && now.second() == 59) {
    water();
  }
  // 7:59:59
  else if (now.hour() == 19 && now.minute() == 59 && now.second() == 59) {
    water();
  }
  // 11:59:59
  else if (now.hour() == 24 && now.minute() == 59 && now.second() == 59) {
    water();
  }
}

void moreWater() {
  delay(200);
  WATERAMOUNT = WATERAMOUNT + 50;
}
  
void lessWater() {
  delay(200);
  WATERAMOUNT = WATERAMOUNT - 50;
}

void water() {
    delay(1000);
    digitalWrite(PUMP, LOW);
    delay(WATERAMOUNT);
    digitalWrite(PUMP, HIGH);
}























