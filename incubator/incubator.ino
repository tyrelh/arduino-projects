#include <PID_v1.h>
#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

#define RelayPin 6

double Setpoint, Input, Output, Max, Min;
byte toggle = 0;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,0,0, DIRECT);

int WINDOWSIZE = 5000;
unsigned long windowStartTime;

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10

// The lcd shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


void setup(void) {

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  pinMode(RelayPin, OUTPUT);
  windowStartTime = millis();

  Setpoint = 3000;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WINDOWSIZE);
  myPID.SetSampleTime(1000);
  
  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  Max = readtemp();
  Min = readtemp();
}


double readtemp(void) {
  int HighByte, LowByte, TReading;
  double Tc_100;
  byte i;
  byte present = 0;
  byte data[12];

  ds.reset();
  ds.skip();
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not

  present = ds.reset();
  ds.skip();    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  return Tc_100;
}


void loop(void) {

  uint8_t buttons = lcd.readButtons();

  // Check buttons
  if (buttons) {
    if (buttons & BUTTON_UP) {
      Setpoint = Setpoint + 100;
    }
    if (buttons & BUTTON_DOWN) {
      Setpoint = Setpoint - 100;
    }
    if (buttons & BUTTON_SELECT) {
      Max = Input;
      Min = Input;
    }
  }

  // Read temperature
  Input = readtemp();

  // Update Min Max temperatures
  if (Input > Max) {
    Max = Input;
  }
  if (Input < Min) {
    Min = Input;
  }
  if (toggle == 0 && 300000 < millis() < 360000) {
     Min = Input;
     toggle = 1;
  }

  // Failsafe
  if (Input > Setpoint) {
    digitalWrite(RelayPin,HIGH);
    delay(5000);
  }

  myPID.Compute();
  
  unsigned long now = millis();
  if (now - windowStartTime>WINDOWSIZE) { //time to shift the Relay Window
    windowStartTime += WINDOWSIZE;
  }

  // Display information to lcd
  lcd.setCursor(0, 0);
  lcd.print("C:");
  lcd.print(Input/100);
  lcd.print(" Hi:");
  lcd.print(Max/100);
  
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(Setpoint/100);
  
  lcd.print(" Lo:");
  lcd.print(Min/100);

  // switch on or off
  if(Output > now - windowStartTime) {
    digitalWrite(RelayPin,LOW);
  }
  else {
    digitalWrite(RelayPin,HIGH);
  }
}
