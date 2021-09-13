#include "lookup.h"





chordShape getData(uint8_t shape) {
  chordShape temp;
  memcpy_P(&temp, &chordLookup[shape - 1], sizeof(chordShape));
  return temp;
}
