#ifndef LOOKUP_H
#define LOOKUP_H

#include <ctype.h>

// SIZE DEFINITIONS
#define TABLESIZE    255
#define MAX_PART_LEN 5
#define MAX_WORD_LEN 12
#define NUM_KEYS     8

enum specialChords {
  BACKSPACE = 0b10001000, 
  NUMTOGGLE = 0b10011001, 
  ENTER = 0b11110000, 
  TAB = 0b00001111
};

/* SHIFT MODES
 *  
 * lower: Will type in all lowercase
 * Camel: Capital first letter
 * UPPER: Types in all caps.
 * UPPER_CYCLE: Same as upper, but returns back to LOWER mode upon a key change instead of Camel
 */
enum ShiftModes { LOWER, UPPER, CAMEL, UPPER_CYCLE };


/* PARTIAL TYPES
 *  
 * LP: Left Partial
 * RP: Right Partial
 * LW: Left Word
 * RW: Right Word
 */
enum partialModes { LP, RP, LW, RW };


/* BACKSPACE MODES {PARTIAL TODO}
 *  
 *  BACK_INOP: Backspace not being pressed.
 * BACK_WAIT: Backspace held, inside delay before BACK_HOLD. If released in this mode, presses backspace. {UNUSED RIGHT NOW}
 * BACK_HOLD: Backspace held, delay is up, holding key.
 */
enum backModes { BACK_INOP, BACK_WAIT, BACK_HOLD };


// character name not necessary, just makes code easier to read.
struct key {
  char name;
  uint8_t pin;
};


// TODO: Backspace mode. If active, won't attempt to check if backspace chord being held.
// Will instead check for the chord to be released. Should also allow for the chord to be pressed again
struct keyboard_obj{
  uint8_t keymap; // Stores all keys currently held. 1 = 'a', 2 = 's', so on
  uint8_t chord; // Stores state of chord, compared against lookup table later. 0 is meaningless, maps to -1 on the lookup table.
  bool    spaceDown     : 1; // space key state storage
  bool    shiftDown     : 1;
  bool    isWord        : 1; // Used as a part of chord.
  bool    numMode       : 1;
  
  //char  backState;
  ShiftModes shiftState : 2;
  backModes  backState  : 2;
  char    bias; // Will be 'l' or 'r' depending on which side of the keyboard began the chord. The bias helps determine which chord from the lookup is active.
  //TODO: Add methods to struct so user can interact with shift state in meaningful way. Methods like, isLower, isCamel, isUpper
};


// LOOKUP TABLE DEFINITION
union chordShape{
  // Character
  struct l{
    char base;
    char baseshift;
  }lett;

  // Word 
  // POTENTIAL TODO: Set as an array of string pointers.
  // Would require some automation or research because
  // I couldn't get PROGMEM to cooperate with references.
  struct d{
    char lp[MAX_PART_LEN];
    char rp[MAX_PART_LEN];
    char lw[MAX_WORD_LEN];
    char rw[MAX_WORD_LEN];
  }dict;
};


chordShape getData(bool isNumMode, uint8_t shapeID);


// CHORD STRUCTS
const chordShape chordLookup[TABLESIZE] PROGMEM = {
  #include "chordLookup.h"
};

const chordShape numLookup[TABLESIZE] PROGMEM = {
  #include "numLookup.h"
};
#endif
