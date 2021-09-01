#ifndef LOOKUP_H
#define LOOKUP_H

#include <ctype.h>

// SIZE DEFINITIONS
#define TABLESIZE    16
#define MAX_PART_LEN 5
#define MAX_WORD_LEN 12
#define NUM_KEYS     8

// SPECIAL KEY DEFINITIONS
#define BACKSPACE 0b10001000 // rightmost letter keys on each hand
#define NUMTOGGLE 0b10011001 // devil horn shape
#define ENTER     0b11110000 // Right hand
#define TAB       0b00001111 // Left hand

// SHIFT MODES
#define LOWER   0
#define UPPER_H 1
#define CAMEL   2
#define CAMEL_H 3
#define UPPER   4
#define LOWER_H 5

// PARTIAL MODES
#define LP 0
#define RP 1
#define LW 2
#define RW 3
/* switch example:
 *  
 *  lower
 *  lower_h break;
 *  camel 
 *  camel_h break;
 *  upper
 *  upper_h break;
 */

// May be removed later, as character name may not be necessary and redundant b/c of lookup table.
struct key {
  char name;
  uint8_t pin;
};

// TODO: Backspace mode. If active, won't attempt to check if backspace chord being held.
// Will instead check for the chord to be released. Should also allow for the chord to be pressed again
struct keyboard_obj{
  uint8_t keymap; // Stores all keys currently held. 1 = 'a', 2 = 's', so on
  uint8_t chord; // Stores state of chord, compared against lookup table later. 0 is meaningless, maps to -1 on the lookup table.
  bool    spaceDown; // stores current state of spaceKey
  bool    isWord; // Used as a part of chord, 
  char    bias; // Will be 'l' or 'r' depending on which side of the keyboard began the chord. The bias helps determine which chord from the lookup is active.
  
  // Special state detectors
  //char  backState; // Unused, stores state of backspace.
  uint8_t shiftState; // 0: lowercase, 1: camel mode. first key caps. 2: caps lock.
  //TODO: Add methods to struct so user can interact with shift state in meaningful way. Methods like, isLower, isCamel, isUpper
};

// LOOKUP TABLE DEFINITION
union chordShape{
  // for case where word is just a character.
  struct l{
    char base;
    char baseshift;
  }lett;

  // Very large structure, necessary evil of progmem.
  struct d{
    char lp[MAX_PART_LEN];
    char rp[MAX_PART_LEN];
    char lw[MAX_WORD_LEN];
    char rw[MAX_WORD_LEN];
  }dict;
};
#endif
