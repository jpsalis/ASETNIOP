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
keyboard_data cur_asetniop;

// Used to compare data as it changes, used for several things.
keyboard_data last_asetniop;


String toPrint;


void setup()
{
  Serial.begin(9600);

  asetniop.chord = 0;
  asetniop.keymap = 0;
  for(int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(pin[i], INPUT);
    asetniop.keymap |= digitalRead(pin[i]) << i;
  }
  Serial.println(asetniop.keyState);
  // Temporary jenk
  toPrint = String();
}

void loop()
{
  next_keymap = 0; // prepare byte for incoming data
  for(int i = 0; i < NUM_KEYS; i++)
  {
    asetniop.keymap |= digitalRead(pin[i]) << i;
  }
  
 


  
  // If button change:
  if(asetniop.keymap != last_asetniop.keymap)
  {
    
    // If no keys currently pressed
    if(asetniop.keymap == 0)
    {
      // compute chord here
      Serial.println("display chord");
      asetniop.chord = 0;
    }

    //TODO: If chord shape is backspace, set flag and start key event.


    


    //TODO: If chord shape is no longer backspace or 1 of the keys was released, end key event


    
    // If all keys off after loop, clear keyBuffer and print char associated with buffer
    for(int i = 0; i < NUM_KEYS; i++)
    {
      toPrint = keyPressed[i];
      if(keyPressed[i])
      {
        Serial.print(pinName[i]);
      }
    }
    Serial.println("");
    last_asetniop = asetniop;
  }


  // can be moved inside the loop later on.
 
  delay(50);
}
