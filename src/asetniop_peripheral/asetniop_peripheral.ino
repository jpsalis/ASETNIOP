// Wire Peripheral Sender
// by Nicholas Zambetti <http://www.zambetti.com>
// Demonsrates use of the Wire library
// Sends data as an I2C/TWI peripheral device
// Refer to the "Wire Master Reader" example for use with this
// Created 29 March 2006
// This example code is in the public domain.

#include <Wire.h>

struct key {
  char name;
  uint8_t pin;
};

const key keys[] = {
  { 'n', 5 }, { 'i', 6 }, { 'o', 7 }, { 'p', 8 }, { ' ', 9 }
};

//space p o i n 
//0000  0 0 0 0

// Condition flipped, most signifigant digit assigned by rightmost button
uint8_t keymap = 0;


void setup() {
  for(auto k : keys) {
    pinMode(k.pin, INPUT_PULLUP);
  }
  Wire.begin(8);                 // join i2c bus with address #8
  Wire.onRequest(requestEvent);  // register event
  
}


void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  keymap = 0;
  for(int i = 0; i < sizeof(keys)/sizeof(key); i++)
  {
    keymap |= !digitalRead(keys[i].pin) << i;
  }
  Wire.write(keymap);
}