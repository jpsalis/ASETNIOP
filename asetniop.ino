#include <ctype.h>
#include "lookup.h"
// C++ code
//
#define NUM_KEYS 10
/*   _____   _____
    |aset | | niop|
    \____*| |*____/
*/


// create bitRegister class, stores 16 bits and allows new ones to be added in a queue-like fashion. Ability to remove not required, but should be fast.
const uint8_t pin[NUM_KEYS] = {
  2, 3, 4, 5,
  6, 8, 
  9, 10, 11, 12
};
const char pinName[NUM_KEYS] = {
  'a', 's', 'e', 't',
  ' ', '^',
  'n', 'i', 'o', 'p'
};

// Stores value of all keys for this loop
bool keyState[NUM_KEYS];

keyboard_state asetniop;

// Stores states of all keys in current chord.
bool keyBuffer[NUM_KEYS];

String toPrint;


void setup()
{
  Serial.begin(9600);
  // For all items in pinout arr, set as input.
  for(int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(pin[i], INPUT);
    // set default values of both arrays. Ensures the initial loop won't cause a key event.
    
  }
  // Temporary jenk
  toPrint = String();
}

void loop()
{
  // Get data from buttons, store in array or bitmap (TODO?)
  for(int i = 0; i < NUM_KEYS; i++)
  {
    keySample = digitalRead(pin[i]); 
    if(keySample != keyState[i]) keyChange = true;
    keyState[i] = keySample;
  }

  // If button change:
  if(keyChange)
  {
    
    // Loop through all buttons, if a key is high, append to keyBuffer.
    for(int i = 0; i < NUM_KEYS; i++)
    {
      
    }


    
    //TODO: If chord shape is backspace, start key event. Set a flag.


    //TODO: If chord shape is no longer backspace or 1 of the keys was released, end key event
    
    // If all keys off after loop, clear keyBuffer and print char associated with buffer
    for(int i = 0; i < NUM_KEYS; i++)
    {
      toPrint = /*String(pinnames[i]) + ": " +*/ keyPressed[i];
      if(keyPressed[i])
      {
        Serial.print(pinName[i]);
      }
    }
    Serial.println("");
  }
  keyChange = false;
  delay(50);
}
