#include "lookup.h"
#include <ctype.h>

/*   
 *  Layout:
 *   _____   _____
 *  |aset | | niop|
 *  \___sp| |sh___/
*/

// PINS
// TODO: may not need the character name in the struct for future reference.
const key keys[NUM_KEYS] = {
  {'a', 2}, {'s', 3}, {'e', 4}, {'t', 5}, 
  {'n', 9}, {'i', 10}, {'o', 11}, {'p', 12}
};

const uint8_t space_pin = 6; 
const uint8_t shift_pin = 8;

// Store state of keyboard. Names self-explanatory.
keyboard_obj asetniop;
keyboard_obj last_asetniop;



void setup()
{
  Serial.begin(9600);
  
  last_asetniop.chord = asetniop.chord = 0;
  last_asetniop.keymap = asetniop.keymap = 0;

  // DECLARING PINMODES:
  pinMode(shift_pin, INPUT);
  pinMode(space_pin, INPUT);
  
  for(int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(keys[i].pin, INPUT);
    // TODO: Possibly make this a function since this line is repeated later.
    asetniop.keymap |= digitalRead(keys[i].pin) << i;
  }
}




void loop()
{
  // UPDATE KEYMAP
  asetniop.keymap = 0; // prepare byte for incoming data
  for(int i = 0; i < NUM_KEYS; i++)
  {
    // set bit in the map
    asetniop.keymap |= digitalRead(keys[i].pin) << i;
  }

  // Set space state if it's held down
  asetniop.

  // DETECT KEYCHANGES
  if(keyDiff(asetniop, last_asetniop))
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    
    // If no keys currently pressed
    if(asetniop.keymap == 0)
    {
      // compute chord here
      Serial.print("chord: ");
      if(numHighBits(asetniop.chord) <= 2)
      {
        Serial.println(getChord(asetniop.chord).lett.base);
      }
      else
      {
        Serial.println(getChord(asetniop.chord).dict.lp);
      }
      asetniop.chord = 0;
      Serial.print("space: ");
      Serial.println(asetniop.spaceDown);
      Serial.println();
    }

    //TODO: If chord shape is backspace, set flag and start key event.


    //TODO: If chord shape is no longer backspace or 1 of the keys was released, end key event.
    last_asetniop = asetniop;
  }

  // TODO: ?Possibly synchronize the output of the keyboard timewise with some sort of delta t. 
  delay(50);
}


//FUNCTIONS:

// Detect differences between
bool keyDiff(keyboard_obj cur, keyboard_obj last)
{
  if (cur.keymap != last.keymap || cur.spaceDown != last.spaceDown) 
    return true;
  
  return false;
}



// Counts number of flipped bits in number and returns to user. there's probably a more efficient way to do this i'm sure, just not sure offhand.
uint8_t numHighBits(uint8_t num)
{
  int r_val = 0;
  for(int i = 0; i < 8; i++)
  {
    if(num & (1 << i)) r_val++;
  }
  return r_val;
}
