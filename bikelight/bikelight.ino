#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, 11);
#define LEFT 9 // pin 9 is the left signal switch
#define RIGHT 8 // pin 8 is the right signal switch
uint32_t blank = strip.Color(0,0,0); // set light state off

void setup(){
  strip.begin(); // start neopixel library
  strip.show(); // set default off state
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
}

void loop(){
  //if (digitalRead(LEFT) == HIGH){
    //signalLeft(strip.Color(0,0,40), strip.Color(40,0,0), 14, 255);
    //rainbow(20);
  //}else if (digitalRead(RIGHT) == HIGH){
    //signalRight(strip.Color(0,0,40), 14, 210);
    //tailLight(strip.Color(255,0,0), 40, 300, 65);
  //}else{
    tailLight(strip.Color(200,0,0), 40, 300, 65);
  //}
}

// TAIL LIGHT
void tailLight(uint32_t c, uint8_t t, uint16_t u, uint8_t v) {
// BLINK 1
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, c);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(t);
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, blank);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, blank);
  }
  strip.show();
  delay(v);
  
// BLINK 2
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, c);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(t);
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, blank);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, blank);
  }
  strip.show();
  delay(v);

// STEADY
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, c);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(u);
  for (uint16_t i = 0; i < 16; i++){
    strip.setPixelColor(i, blank);
  }
  for (uint16_t i = 31; i > 15; i--){
    strip.setPixelColor(i, blank);
  }
  strip.show();
  delay(380);
}

// SIGNAL RIGHT
void signalRight(uint32_t c, uint8_t t, uint8_t u){
  for (uint16_t i = 28; i > 18; i--){
    strip.setPixelColor(i, c);
    strip.show();
    delay(t);
  }
  delay(u);
  for (uint16_t i = 28; i > 18; i--){
    strip.setPixelColor(i, blank);
    strip.show();
    delay(t);
  }
  delay(u);
}

// SIGNAL LEFT
void signalLeft(uint32_t c, uint32_t d, uint8_t t, uint8_t r){
  for (uint16_t i = 3; i < 13; i++){
    strip.setPixelColor(i, c);
    strip.show();
    delay(t);
  }
  delay(r);
  for (uint16_t i = 3; i < 13; i++){
    strip.setPixelColor(i, blank);
    strip.show();
    delay(t);
  }
  delay(r);
}

// RAINBOW
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
