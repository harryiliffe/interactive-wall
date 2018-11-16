// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define NUM_NODES 2

struct Node {
  uint8_t address;
  uint8_t ledState;
  uint8_t buttonState;
};


Node nodes[NUM_NODES] = {
  {0, 1, 0},
  {1, 0, 0}
};

int leds[NUM_NODES] = {0,1};

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {

  for (int i = 0; i < NUM_NODES; i++) {
    Wire.requestFrom(nodes[i].address, 2);    // request 2 bytes from slave device #8
    while (Wire.available()) { // slave may send less than requested
      nodes[i].ledState = Wire.read();
      nodes[i].buttonState = Wire.read();
    }

    Serial.print("NODE ");
    Serial.print(i);
    Serial.print(":\t");
    Serial.print(nodes[i].ledState);
    Serial.print("\t");
    Serial.println(nodes[i].buttonState);
    
  }
  Serial.println("");
  
  delay(1000);
  for (int i = 0; i < NUM_NODES; i++) {
    leds[i] = !leds[i];
    sendLED(nodes[i].address, leds[i]);
  }
}


void sendLED(byte address, bool state) {
  Serial.println("Setting Node " +String(address)+" led to " +String(state));
  Wire.beginTransmission(address);
  Wire.write(0); //address
  Wire.write(state);
  Wire.endTransmission();
}


