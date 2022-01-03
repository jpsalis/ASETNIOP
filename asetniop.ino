#include "lookup.h"
#include <ctype.h>
#include <Keyboard.h>
/*  Layout:  
 *       ◻
 *       |
 *    ___|_     _____
 *   |aset |===| niop|
 *   \___sp|   |sh___/
 *
 * Left half will be host, connected to PC,
 * Right half will be the peripheral, providing keyData to left half.
 * Connection will be I^2C.
  */
// PINS
// Character name is just for reference against diagram above.
const key keys[NUM_KEYS] = {
  {'a', A4}, {'s', 7}, {'e', 9}, {'t', 10},
  {'n', A0}, {'i', A1}, {'o', A2}, {'p', A3}
};

const uint8_t shift_pin = 11;
const uint8_t space_pin = 12;


// Store state of keyboard. Names self-explanatory.
keyboard_obj asetniop;
keyboard_obj last_asetniop;

void setup()
{
  Keyboard.begin();

  last_asetniop.chord = asetniop.chord = 0;
  last_asetniop.keymap = asetniop.keymap = 0;

  // DECLARING PINMODES:
  pinMode(LED_BUILTIN, OUTPUT);
                                                                                                                                                                                                 
  // SET KEYS AS ACTIVE LOW: 
  pinMode(shift_pin, INPUT_PULLUP);

  
  pinMode(space_pin, INPUT_PULLUP);

  
  for (int i = 0; i < NUM_KEYS; i++)
  {
    pinMode(keys[i].pin, INPUT_PULLUP);    
  }

  // FLASH LED WHEN FINISHED SETTING UP
  for (int i = 0; i < 6; i++)
  {
    digitalWrite(LED_BUILTIN, (i % 2 == 0) ? HIGH : LOW);
    delay(30);
  }
}


void loop()
{
  // UPDATE KEYMAP
  asetniop.keymap = 0;
  // Loop through keys
  for (int i = 0; i < NUM_KEYS; i++)
  {
    // set bit in the map
    asetniop.keymap |= !digitalRead(keys[i].pin) << i;
  }

  asetniop.spaceDown = !digitalRead(space_pin);
  asetniop.shiftDown = !digitalRead(shift_pin);


  // DETECT KEYCHANGES:
  if (keyDiff(&asetniop, &last_asetniop))
  {
    // append any new keys to the chord.
    asetniop.chord |= asetniop.keymap;
    asetniop.isWord |= asetniop.spaceDown;


    // If no keys currently pressed
    if (!keyHeld(&asetniop))
    {
      // PRINT CHORD:
      if (asetniop.chord != 0)
      {
        // DETERMINE MODE
        switch (asetniop.chord)
        {
          case ENTER:
            Keyboard.write(KEY_RETURN);
            break;

          case BACKSPACE:
            // Code used to print here, now printed with different code. Case remains because the computer 
            // will still try to treat it like a normal chord through "default:" even if it's a special case.
            break;

          case TAB:
            Keyboard.write(KEY_TAB);
            break;

          case NUMTOGGLE:
            asetniop.numMode = !asetniop.numMode;
            digitalWrite(LED_BUILTIN, (asetniop.numMode) ? HIGH : LOW);
            break;

          default:
            // Letter
            putChord(asetniop, getData(asetniop.numMode, asetniop.chord));
        }

        // If shiftstate is UPPER or UPPER_CYCLE, change to UPPER_CYCLE.
        // Else, change shiftstate to LOWER.
        asetniop.shiftState = (asetniop.shiftState % 0b10 == 1 ? UPPER_CYCLE : LOWER);

        asetniop.chord = 0;
        asetniop.bias = '\0';
      }

      // PRINT SPACE:
      if (asetniop.isWord)
      {
        Keyboard.write(' ');
        asetniop.isWord = false;
      }
    }

    // SET BIAS FOR NEW CHORD:
    else if (last_asetniop.chord == 0 && asetniop.chord != 0)
    {
      asetniop.bias = asetniop.chord >= 0x10 ? 'r' : 'l';
    }

    // BACKSPACE HOLD:
    if (asetniop.backState == BACK_INOP 
        && (asetniop.chord == BACKSPACE && asetniop.keymap == BACKSPACE))
    {
      Keyboard.press(KEY_BACKSPACE);
      asetniop.backState = BACK_HOLD;
    }
    
    // BACKSPACE RELEASE:
    else if (asetniop.backState == BACK_HOLD 
             && (asetniop.chord != BACKSPACE || asetniop.keymap != BACKSPACE))
    {
      Keyboard.release(KEY_BACKSPACE);
      asetniop.backState = BACK_INOP;
    }
        
  }
  
  // SET SHIFTSTATE
  if (asetniop.shiftDown != last_asetniop.shiftDown)
  {
    asetniop.shiftState = (ShiftModes)(((uint8_t)asetniop.shiftState + 1) % 4);
  }

  // this operation is performed every tick which is unecessary but it reduces code repetition
  last_asetniop = asetniop;
  delay(10);
}



// HELPER FUNCTIONS:

// Check if any key differences observed.
bool keyDiff (const keyboard_obj *cur, const keyboard_obj *last)
{
  return (cur->keymap != last->keymap
          || cur->spaceDown != last->spaceDown);
}

bool keyHeld (const keyboard_obj *a)
{
  return a->spaceDown || a->keymap;
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

   FUNCTION: Display relavant character, chord or partial based on a set precedence if the primary doesn't exist.
   0b00: lp
   0b01: rp
   0b10: lw
   0b11: rw
*/
bool putChord(const keyboard_obj keyData, const chordShape chordData) {

  // PRINT IF CHARACTER
  if (numHighBits(keyData.chord) <= 2 || keyData.numMode)
  {
    char temp;

    if (keyData.shiftState == LOWER) 
    {
      temp = getData(asetniop.numMode, asetniop.chord).lett.base;
    }
    else
    {
      temp = getData(asetniop.numMode, asetniop.chord).lett.baseshift;
    }
    Keyboard.write(temp);
    return true;
  }

  // ELSE ASSUME WORD
  uint8_t primary = 0;
  primary |= (keyData.bias == 'r');
  primary |= keyData.isWord << 1;

  /* xor's the primary recursively 0 to 4
     gives the priority order I want for every possible primary.
  */
  String output;
  for (uint8_t i = 0; i < 4; i++)
  {
    switch (primary ^ i)
    {
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
    // Check if current chord is valid
    if (output != "")
    {
      // SET CASE
      switch (keyData.shiftState)
      {
        case LOWER:
          break;

        case CAMEL:
          output[0] = toupper(output[0]);
          break;

        // Every other case is UPPERCASE in one form or another
        default:
          for (int i = 0; i < output.length(); i++)
            output[i] = toupper(output[i]);
          break;
      }

      // PRINT RESULT
      Keyboard.print(output);

      return true;
    }
  }
  return false;
}
