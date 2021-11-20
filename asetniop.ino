#include "lookup.h"
#include <ctype.h>
#define KEYBOARD
#ifdef KEYBOARD
  #include <Keyboard.h>
#endif
/*   
 *  Layout:
 *   _____   _____
 *  |aset | | niop|
 *  \___sp| |sh___/
*/

// PINS
// TODO: may not need the character name in the struct for future reference.
const key keys[NUM_KEYS] = {
  {'a', 5}, {'s', 7}, {'e', 9}, {'t', 10}, 
  {'n', A0}, {'i', A1}, {'o', A2}, {'p', A3}
};

const uint8_t shift_pin = 11;
const uint8_t space_pin = 12; 


// Store state of keyboard. Names self-explanatory.
keyboard_obj asetniop;
keyboard_obj last_asetniop;

void setup()
{
 // Temporary test
  //delay(1000);
  //Serial.begin(9600);
  #ifdef KEYBOARD
  Keyboard.begin(); 
  #endif
  
  last_asetniop.chord = asetniop.chord = 0;
  last_asetniop.keymap = asetniop.keymap = 0;

  // DECLARING PINMODES:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(shift_pin, INPUT);
  pinMode(space_pin, INPUT);
  
  for(int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(keys[i].pin, INPUT);
    // TODO: Possibly make this a function since this line is repeated later.
    asetniop.keymap |= digitalRead(keys[i].pin) << i;
  }
  digitalWrite(LED_BUILTIN, HIGH);
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
  asetniop.shiftDown = digitalRead(shift_pin);

  // DETECT KEYCHANGES
  if(keyDiff(asetniop, last_asetniop))
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    asetniop.isWord |= asetniop.spaceDown;


    // If no keys currently pressed
    if(!keyHeld(asetniop))
    { 
      // PRINT CHORD:      
      if(asetniop.chord != 0) {

        // DETERMINE MODE
        switch(asetniop.chord)
        {
          case ENTER:         
            #ifdef KEYBOARD
              Keyboard.write(KEY_RETURN);
            #else
              Serial.print("__ENTER__");
            #endif
          break;
            
          case BACKSPACE:
            #ifdef KEYBOARD
              Keyboard.write(KEY_BACKSPACE);
            #else
              Serial.print("__BACKSPACE__");
            #endif
          break;

          case TAB:
            #ifdef KEYBOARD
              Keyboard.write(KEY_TAB);
            #else
              Serial.print("__TAB__");
            #endif
          break;
            
          case NUMTOGGLE:
            /* TODO */
            asetniop.numMode = !asetniop.numMode;
            Serial.print("__TOGGLE__");
          break;
            
          
            
          default:
            // Letter
            if(numHighBits(asetniop.chord) <= 2 || asetniop.numMode)
            {  
              
              #ifdef KEYBOARD
                Keyboard.write(getData(asetniop.numMode, asetniop.chord).lett.base);
              #else
                Serial.print(  getData(asetniop.numMode, asetniop.chord).lett.base);
              #endif
            }
            else
            {
              // Word
              putChord(asetniop, getData(false, asetniop.chord));
            } 
        }

        asetniop.chord = 0;
        // TODO: Determine if necessary.
        asetniop.bias = '\0';
      }

      // PRINT SPACE
      if(asetniop.isWord)
      {
        #ifdef KEYBOARD
              Keyboard.write(' ');
        #else
              Serial.print(" ");
        #endif
        asetniop.isWord = false;
      }
    }
    
    // Set bias of keyboard to l or r based on value of aset.chord if a new chord has been started
    else if (last_asetniop.chord == 0 && asetniop.chord != 0)
    {
      asetniop.bias = asetniop.chord >= 0x10 ? 'r' : 'l';
    }

    //TODO: If chord shape is backspace, set flag and start key event.
    

    //TODO: If chord shape is no longer backspace or 1 of the keys was released, end key event.
    
    last_asetniop = asetniop;
  }
  // TODO?: Possibly synchronize the output of the keyboard timewise with some sort of delta t. 
  delay(50);
}



//FUNCTIONS:

// Check if any key differences observed.
bool keyDiff (keyboard_obj cur, keyboard_obj last)  {  return (cur.keymap != last.keymap || cur.spaceDown != last.spaceDown);  }
bool keyHeld (keyboard_obj a)  {  return a.spaceDown || a.keymap;  }



/*
 * Given an 8 bit number, returns the number of high bits.
 */
uint8_t numHighBits(uint8_t num)
{
  int toReturn = 0;
  for(int i = 0; i < 8; i++)  if(num & (1 << i))  toReturn++;
  return toReturn;
}


/* 
 * Returns the position of the first bit it finds in the byte.
 * 
 * RETURN VALUE:
 * 0000 0000 
 * 0123 4567 -1(Error)
 */
char firstHighBit(uint8_t num)
{
  // Magic, magic single-line for and if statements
  for(int i = 0; i < 8; i++)  if (num & (1 << i))  return i;
  // No bit found, inform caller
  return -1;
}


/*
 * PARAMS:
 *   - keyData: State of keyboard.
 *   - chordData: copy of a chord from lookup table
 *   
 * FUNCTION: Display relavant chord or partial in a specific order if the primary doesn't exist.  
 * 0b00: lp
 * 0b01: rp
 * 0b10: lw
 * 0b11: rw
 */
bool putChord(const keyboard_obj keyData, const chordShape chordData) {
  uint8_t primary = 0;
  primary |= (keyData.bias == 'r');
  primary |= keyData.isWord << 1;

  /* this is some clever bit of math I'm pretty proud of. 
   * xor'ing the primary state with the digits 0-4 gives me the priority order I want for every possible primary.
   * Imagine primary is 01, or RP. Order for others if no primary exists there should then go: 
   * RP (01), LP (00), RW (10)then LW (11). This behavior allows this design to function as intended.
   */
  String output;
  for(uint8_t i = 0; i < 4; i++)
  {
    switch (primary ^ i) {
      case LP:
        output = String(chordData.dict.lp);
        break;
      case RP:
        output = String(chordData.dict.rp);
        break;
      case LW:
        output = String(chordData.dict.lw);
        break;
      case RW:
        output = String(chordData.dict.rw);
        break;
    }
    
    // Verify value
    if (output != "") 
    {
      #ifdef KEYBOARD
        Keyboard.print(output);
      #else
        Serial.print(output);
      #endif
      return true;
    }
  }
  // No chord found, error of some kind. Print some other way.
  return false;
}
