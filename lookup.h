#ifndef LOOKUP_H

#define LOOKUP_H
#include<ctype.h>

/* special modes:
 *  '
 */
struct keyboard_data{
  uint8_t chord; // Stores state of chord, compared against lookup table later.
  uint8_t keymap; // Stores all keys currently held. 1 = 'a', 2 = 's', so on
  
  
  
  uint8_t shiftMode; // Used for special modes, 
  bool spacePressed;
  bool bckspPressed; // May not need
 };



#endif
