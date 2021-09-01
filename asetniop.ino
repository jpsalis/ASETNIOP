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
  
  asetniop.spaceDown = digitalRead(space_pin);
  

  // DETECT KEYCHANGES
  if(keyDiff(asetniop, last_asetniop))
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    asetniop.isWord |= asetniop.spaceDown;


    // If no keys currently pressed
    if(!keyHeld(asetniop))
    {
      // PRINT CHORD
      if(asetniop.chord != 0) {
        Serial.print("chord: ");
        if(numHighBits(asetniop.chord) <= 2)
        {
          Serial.println(getData(asetniop.chord).lett.base);
        }
        else
        {
          char * temp;
          getChord(getData(asetniop.chord), temp);

          else(
            Serial.println(getData(asetniop.chord).dict.lp);
        }
        asetniop.chord = 0;
      }

      // PRINT SPACE
      if(asetniop.isWord)
      {
        Serial.print("space down");
        asetniop.isWord = false;
      }
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



// Check if any key differences observed.
bool keyDiff(keyboard_obj cur, keyboard_obj last) { return (cur.keymap != last.keymap || cur.spaceDown != last.spaceDown); }



bool keyHeld(keyboard_obj a) {return a.spaceDown || a.keymap;}



// Counts number of flipped bits in number and returns to user. there's probably a more efficient way to do this i'm sure, just not sure offhand.
uint8_t numHighBits(uint8_t num)
{
  int toReturn = 0;
  for(int i = 0; i < 8; i++)
  {
    if(num & (1 << i)) toReturn++;
  }
  return toReturn;
}



// Given an index, return a string that will be printed to the user. May need to add additional functionality later on.

/* primary states:
 * 0b00: lp
 * 0b01: rp
 * 0b10: lw
 * 0b11: rw
 */

/* 
 *  Primary: Number from 0 to 4 indicating the 
 */
bool putChord(const chordShape data) {
  uint8_t primary = 0;
  primary |= (asetniop.primary == 'r');
  primary |= asetniop.isWord << 1;

  /* this is some clever bit of math I'm pretty proud of. 
   *  xor'ing my primary state with the digits 0-4 gives me the priority order I want for every possible primary.
   * Imagine primary is 01, or RP. Order for others if no primary exists there should then go: 
   * RP (01), LP (00), RW (10)then LW (11). This behavior allows this design to function as intended.
   */
  String output;
  for(uint8_t i = 0; i < 4; i++)
  {
    switch (primary ^ i) {
      case LP:
        output = String(data.lp);
        break;
      case RP:
        output = String(data.rp);
        break;
      case LW:
        output = String(data.lw);
        break;
      case RW:
        output = String(data.rw);
        break;
    }
    // Verify value
    if (output != "") return true 
  }
  // No chord found, error of some kind. Print some other way.
  return false;
}
