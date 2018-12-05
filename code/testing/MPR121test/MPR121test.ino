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
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define HUBS 2
int addresses[4] = {0x5A, 0x5B, 0x5C, 0x5D};
// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap[HUBS] = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched[HUBS];
uint16_t currtouched[HUBS];

void setup() {
  Serial.begin(115200);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  for (int i = 0; i < HUBS; i++) {
    if (!cap[i].begin(addresses[i])) {
      Serial.print( addresses[i], 16);
      Serial.println("   MPR121 not found, check wiring?");
      while (1);
    }
    Serial.print( addresses[i], 16);
    Serial.println(":  MPR121 found!");
  }
}

void loop() {
  for (int i = 0; i < HUBS; i++) {
    // Get the currently touched pads
    currtouched[i] = cap[i].touched();

    for (uint8_t l = 0; l < 12; l++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched[i] & _BV(l)) && !(lasttouched[i] & _BV(l)) ) {
        Serial.print(i); Serial.print(":  ");Serial.print(l); Serial.println(" touched");
      }
      // if it *was* touched and now *isnt*, alert!
      if (!(currtouched[i] & _BV(l)) && (lasttouched[i] & _BV(l)) ) {
        Serial.print(i); Serial.print(":  ");Serial.print(l); Serial.println(" released");
      }
    }

    // reset our state
    lasttouched[i] = currtouched[i];
  }
}
