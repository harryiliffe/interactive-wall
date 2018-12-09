/*********************************************************
  This is a library for the MPR121 12-channel Capacitive touch sensor

  Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/

  These sensors use I2C communicate, at least 2 pins are required
  to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>


#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define MPR121_I2CADDR_DEFAULT 0x5B ///< default I2C address
// You can have up to 4 on one i2c bus but one is enough for testing!
#define HUBS 2

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched[HUBS];
uint16_t currtouched[HUBS];
int8_t _i2caddr[HUBS];

enum {
  MPR121_TOUCHSTATUS_L = 0x00,
  MPR121_TOUCHSTATUS_H = 0x01,
  MPR121_FILTDATA_0L  = 0x04,
  MPR121_FILTDATA_0H  = 0x05,
  MPR121_BASELINE_0   = 0x1E,
  MPR121_MHDR         = 0x2B,
  MPR121_NHDR         = 0x2C,
  MPR121_NCLR         = 0x2D,
  MPR121_FDLR         = 0x2E,
  MPR121_MHDF         = 0x2F,
  MPR121_NHDF         = 0x30,
  MPR121_NCLF         = 0x31,
  MPR121_FDLF         = 0x32,
  MPR121_NHDT         = 0x33,
  MPR121_NCLT         = 0x34,
  MPR121_FDLT         = 0x35,

  MPR121_TOUCHTH_0    = 0x41,
  MPR121_RELEASETH_0    = 0x42,
  MPR121_DEBOUNCE = 0x5B,
  MPR121_CONFIG1 = 0x5C,
  MPR121_CONFIG2 = 0x5D,
  MPR121_CHARGECURR_0 = 0x5F,
  MPR121_CHARGETIME_1 = 0x6C,
  MPR121_ECR = 0x5E,
  MPR121_AUTOCONFIG0 = 0x7B,
  MPR121_AUTOCONFIG1 = 0x7C,
  MPR121_UPLIMIT   = 0x7D,
  MPR121_LOWLIMIT  = 0x7E,
  MPR121_TARGETLIMIT  = 0x7F,

  MPR121_GPIODIR  = 0x76,
  MPR121_GPIOEN   = 0x77,
  MPR121_GPIOSET  = 0x78,
  MPR121_GPIOCLR  = 0x79,
  MPR121_GPIOTOGGLE  = 0x7A,

  MPR121_SOFTRESET = 0x80,
};


void setup() {
  Serial.begin(115200);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch and button test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D

  
  if (!begin(0x5A, 16,17)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");


}

void loop() {
  // Get the currently touched pads
  currtouched = touched();

  for (uint8_t i = 0; i < 13; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }


  // reset our state
  lasttouched = currtouched;
}


bool begin(uint8_t i2caddr, int sda, int scl){
  Wire.begin();
    
  _i2caddr = i2caddr;

  // soft reset
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  for (uint8_t i=0; i<0x7F; i++) {
   Serial.print("$"); Serial.print(i, HEX); 
   Serial.print(": 0x"); Serial.println(readRegister8(i));
  }
  

  writeRegister(MPR121_ECR, 0x0);

  uint8_t c = readRegister8(MPR121_CONFIG2);
  
  if (c != 0x24) return false;


  setThresholds(12, 6);
  writeRegister(MPR121_MHDR, 0x01);
  writeRegister(MPR121_NHDR, 0x01);
  writeRegister(MPR121_NCLR, 0x0E);
  writeRegister(MPR121_FDLR, 0x00);

  writeRegister(MPR121_MHDF, 0x01);
  writeRegister(MPR121_NHDF, 0x05);
  writeRegister(MPR121_NCLF, 0x01);
  writeRegister(MPR121_FDLF, 0x00);

  writeRegister(MPR121_NHDT, 0x00);
  writeRegister(MPR121_NCLT, 0x00);
  writeRegister(MPR121_FDLT, 0x00);

  writeRegister(MPR121_DEBOUNCE, 0);
  writeRegister(MPR121_CONFIG1, 0x10); // default, 16uA charge current
  writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

//  writeRegister(MPR121_AUTOCONFIG0, 0x8F);

//  writeRegister(MPR121_UPLIMIT, 150);
//  writeRegister(MPR121_TARGETLIMIT, 100); // should be ~400 (100 shifted)
//  writeRegister(MPR121_LOWLIMIT, 50);
  // enable all electrodes
  writeRegister(MPR121_ECR, 0x8F);  // start with first 5 bits of baseline tracking

  return true;
}

void setThresholds(uint8_t touch, uint8_t release) {
  for (uint8_t i=0; i<12; i++) {
    writeRegister(MPR121_TOUCHTH_0 + 2*i, touch);
    writeRegister(MPR121_RELEASETH_0 + 2*i, release);
  }
}

uint16_t filteredData(uint8_t t) {
  if (t > 12) return 0;
  return readRegister16(MPR121_FILTDATA_0L + t*2);
}


uint16_t baselineData(uint8_t t) {
  if (t > 12) return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

uint16_t touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}

uint8_t readRegister8(uint8_t reg) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr, 1);
    if (Wire.available() < 1)
      return 0;
    return (Wire.read());
}

uint16_t readRegister16(uint8_t reg) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr, 2);
    if (Wire.available() < 2)
      return 0;
    uint16_t v = Wire.read();
    v |=  ((uint16_t) Wire.read()) << 8;
    return v;
}

void writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)(value));
    Wire.endTransmission();
}
