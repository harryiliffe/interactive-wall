// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int ledPin = 13;
uint8_t ledState;

int buttonPin = 10;
uint8_t buttonState = 1;

uint8_t Byte[2];

void setup() {
  Wire.begin(0);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);  // start serial for output
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.println("Slave Setup!");
}

void loop() {
  buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
  digitalWrite(ledPin, ledState);
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(ledState);
  Wire.write(buttonState);
}

void receiveEvent(int count) {
  Serial.println("received event");
  while (1 < Wire.available()) { // loop through all but the last
    Byte[0] = Wire.read();
    Byte[1] = Wire.read();
  }
  Serial.print(Byte[0]);
  Serial.print("\t");
  Serial.println(Byte[1]);
  
  switch (Byte[0]){
    case 0:
      ledState = Byte[1];
      break;
    case 1:
      break;
  }
}
