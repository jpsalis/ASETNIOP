#include <ctype.h>
#include "lookup.h"
// C++ code
//
#define NUM_KEYS 8
/*   _____   _____
    |aset | | niop|
    \____*| |*____/
*/


// TODO: may not need the character name in the struct for future reference.
const key keys[NUM_KEYS] = {
  {'a', 2}, {'s', 3}, {'e', 4}, {'t', 5}, 
  {'n', 9}, {'i', 10}, {'o', 11}, {'p', 12}
};

// Plan to think of more permanent solution to this, but as a special case, it makes sense to give them their own pins.
const uint8_t shift_pin = 6;
const uint8_t space_pin = 8; 

// Store state of keyboard. Names self-explanatory.
keyboard_obj asetniop;
keyboard_obj last_asetniop;



void setup()
{
  Serial.begin(9600);

  last_asetniop.chord = asetniop.chord = 0;
  last_asetniop.keymap = asetniop.keymap = 0;
  for(int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(keys[i].pin, INPUT);
    asetniop.keymap |= digitalRead(keys[i].pin) << i;
  }
  Serial.println(asetniop.keymap);
}

void loop()
{
  // UPDATE KEYMAP
  uint8_t temp;
  asetniop.keymap = 0; // prepare byte for incoming data
  for(int i = 0; i < NUM_KEYS; i++)
  {
    // set bit in the map
    temp = digitalRead(keys[i].pin) << i;
    asetniop.keymap |= temp;
  }

  
  // DETECT KEYCHANGES
  if(asetniop.keymap != last_asetniop.keymap)
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    
    // If no keys currently pressed
    if(asetniop.keymap == 0)
    {
      // compute chord here
      Serial.print("chord: ");
      Serial.println(asetniop.chord, BIN);
      asetniop.chord = 0;
    }

    //TODO: If chord shape is backspace, set flag and start key event.


    //TODO: If chord shape is no longer backspace or 1 of the keys was released, end key event.
    last_asetniop = asetniop;
  }


  // can be moved inside the loop later on.
 
  delay(50);
}
