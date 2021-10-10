#include "lookup.h"

// TODO: If chord mode is numberpad, return 
chordShape getData(uint8_t shape) {
  chordShape temp;
  memcpy_P(&temp, &chordLookup[shape - 1], sizeof(chordShape));
  return temp;
}
