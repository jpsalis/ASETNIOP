#include "lookup.h"
#include <ctype.h>

#define KEYBOARD

// Creating these macros for testing purposes. May remove them at a later date.
#ifdef KEYBOARD
  #include <Keyboard.h>
  #define writeChord(val) Keyboard.write(val)
  #define printChord(val) Keyboard.print(val)
#else
  #define writeChord(val) Serial.print(val)
  #define printChord(val) Serial.println(val)
#endif
/*
    Layout:
     _____   _____
    |aset | | niop|
    \___sp| |sh___/
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
#ifdef KEYBOARD
  Serial.begin(9600);
  Keyboard.begin();
#else
  
#endif

  last_asetniop.chord = asetniop.chord = 0;
  last_asetniop.keymap = asetniop.keymap = 0;

  // DECLARING PINMODES:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(shift_pin, INPUT);
  pinMode(space_pin, INPUT);

  for (int i = 0; i < NUM_KEYS; i++)
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
  asetniop.keymap = 0;
  // Loop through keys
  for (int i = 0; i < NUM_KEYS; i++)
  {
    // set bit in the map
    asetniop.keymap |= digitalRead(keys[i].pin) << i;
  }

  asetniop.spaceDown = digitalRead(space_pin);
  asetniop.shiftDown = digitalRead(shift_pin);


  // DETECT KEYCHANGES
  if (keyDiff(asetniop, last_asetniop))
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    asetniop.isWord |= asetniop.spaceDown;


    // If no keys currently pressed
    if (!keyHeld(asetniop))
    {
      // PRINT CHORD:
      if (asetniop.chord != 0)
      {
        // DETERMINE MODE
        switch (asetniop.chord)
        {
          case ENTER:
            writeChord(KEY_RETURN);
            break;

          case BACKSPACE:
            writeChord(KEY_BACKSPACE);
            break;

          case TAB:
            writeChord(KEY_TAB);
            break;

          case NUMTOGGLE:
            asetniop.numMode = !asetniop.numMode;
            break;

          default:
            // Letter
            putChord(asetniop, getData(asetniop.numMode, asetniop.chord));
        }

        // DEBUG
        Serial.println("Shift Change");
        asetniop.shiftState = (asetniop.shiftState % 0b10 == 1 ? UPPER_CYCLE : LOWER);
        Serial.println(asetniop.shiftState);

        asetniop.chord = 0;
        // TODO: Determine if necessary.
        asetniop.bias = '\0';
      }

      // PRINT SPACE
      if (asetniop.isWord)
      {
        writeChord(' ');
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


    // TODO: Adding on every change instead of relying on the actual state could cause an issue,
    // might want to have this set more statically in the future, and assign it separately from the main key checker
    if (asetniop.shiftDown != last_asetniop.shiftDown)
    {
      asetniop.shiftState = (ShiftModes)(((uint8_t)asetniop.shiftState + 1) % 4);
      Serial.println(asetniop.shiftState);
    }



    last_asetniop = asetniop;
  }
  // TODO?: Possibly synchronize the output of the keyboard timewise with some sort of delta t.
  delay(50);
}



//FUNCTIONS:

// Check if any key differences observed.
bool keyDiff (keyboard_obj cur, keyboard_obj last)
{
  return (cur.keymap != last.keymap ||
          cur.spaceDown != last.spaceDown ||
          cur.shiftDown != last.shiftDown);
}

bool keyHeld (keyboard_obj a)  {
  return a.spaceDown || a.keymap;
}

uint8_t numHighBits(uint8_t num)
{
  int toReturn = 0;
  for (int i = 0; i < 8; i++)  if (num & (1 << i))  toReturn++;
  return toReturn;
}


/*
   Returns the position of the first bit it finds in the byte.

   RETURN VALUE:
   0000 0000
   0123 4567 -1(Error)
*/
char firstHighBit(uint8_t num)
{
  // Magic, magic single-line for and if statements
  for (int i = 0; i < 8; i++)  if (num & (1 << i))  return i;
  // No bit found, inform caller
  return -1;
}


/*
   PARAMS:
     - keyData: State of keyboard.
     - chordData: copy of a chord from lookup table

   FUNCTION: Display relavant chord or partial in a specific order if the primary doesn't exist.
   0b00: lp
   0b01: rp
   0b10: lw
   0b11: rw
*/
bool putChord(const keyboard_obj keyData, const chordShape chordData) {
  
  // IF CHORD IS CHAR
  if (numHighBits(keyData.chord) <= 2 || keyData.numMode)
  {
    char temp = getData(asetniop.numMode, asetniop.chord).lett.base;
    if (keyData.shiftState != LOWER)    temp = toupper(temp);
    
    writeChord(temp);
    return true;
  }

  // ELSE PRINT THE WORD WITH MAGIC
  uint8_t primary = 0;
  primary |= (keyData.bias == 'r');
  primary |= keyData.isWord << 1;

  /* this is some clever bit of math I'm pretty proud of.
     xor'ing the primary state with the digits 0-4 gives me the priority order I want for every possible primary.
     Imagine primary is 0b01, or RP. Order for others if no primary exists there should then go:
     RP (01), LP (00), RW (10)then LW (11). This behavior allows this design to function as intended.
  */
  String output;
  for (uint8_t i = 0; i < 4; i++)
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
      switch (keyData.shiftState)
      {
        case LOWER:
          break;

        case CAMEL:
          output[0] = toupper(output[0]);
          break;

        // Every other case is uppercase in one form or another
        default:
          for(int i = 0; i < output.length(); i++) 
            output[i] = toupper(output[i]);
          break;
      }
      // TODO: Convert word based on shift state. 
      printChord(output);
      return true;
    }
  }
  // No chord found, error of some kind. Print some other way.
  return false;
}
