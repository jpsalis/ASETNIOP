#ifndef LOOKUP_H
  #define LOOKUP_H
  
  #include <ctype.h>
  #define SPACE

  // allows easier access to data in the table, and easier to alter.
  struct key {
    char name;
    uint8_t pin;
  };
  
  
  
  struct keyboard_obj{
    uint8_t chord; // Stores state of chord, compared against lookup table later.
    uint8_t keymap; // Stores all keys currently held. 1 = 'a', 2 = 's', so on
    
    // Other state detectors
    char primary; // stores the state
    uint8_t shiftState; // 0: normal lowercase, 1: 1st char capital, reset after chord 
    bool spaceDown;
    
  };



  struct key_lookup{
    char *lp;
    char *rp;
    char *lw;
    char *rw;
  }


#endif
