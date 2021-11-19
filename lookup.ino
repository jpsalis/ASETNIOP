#include "lookup.h"

// TODO: If chord mode is numberpad, return 
chordShape getData(bool isNumMode, uint8_t shape) {
  chordShape temp;
  if (!isNumMode) 
  {
    memcpy_P(&temp, &chordLookup[shape - 1], sizeof(chordShape));
  }
  return temp;
}
