#ifndef LOOKUP_H
#define LOOKUP_H
  
  #include <ctype.h>

  // MACROS:
  // Change to 255 for full development once table is prepped.
  #define TABLESIZE 16
  
  
  #define NUM_KEYS  8
  #define BACKSPACE 0b10001000 // 
  #define NUMTOGGLE 0b10011001 // devil horn shape
  #define ENTER     0b11110000 // Right hand
  #define TAB       0b00001111 // Left hand
  
  // May be removed later, as character name may not be necessary and redundant b/c of lookup table.
  struct key {
    char name;
    uint8_t pin;
  };

  
  // TODO: Backspace mode. If active, won't attempt to check if backspace chord being held.
  // Will instead check for the chord to be released. Should also allow for the chord to be pressed again
  struct keyboard_obj{
    uint8_t keymap; // Stores all keys currently held. 1 = 'a', 2 = 's', so on
    uint8_t chord; // Stores state of chord, compared against lookup table later.
    
    // Other state detectors
    //char  backState; // Unused, stores state of backspace. Backspace has 3 modes: 
    bool    spaceDown;
    uint8_t shiftState; // 0: normal lowercase, 1: 1st char capital, reset after chord. Holding  2, a sort of caps lock. Press resets to 0.
  };



  // UNTESTED, but concept is that the union can either store a single char and its shifted variant, or a bunch of string references
  union chordShape{
    struct l{
      char base;
      char baseshift;
    }lett;

    struct d{
      char *lp;
      char *rp;
      char *lw;
      char *rw;
    }dict;
  };
#endif
