#include "lookup.h"

// TODO: If chord mode is numberpad, return 
chordShape getData(bool isNumMode, uint8_t shapeID) {
  const chordShape *tempref;
  chordShape temp;

  // Pick table based on number mode selected
  tempref = isNumMode ? &numLookup[shapeID - 1] : &chordLookup[shapeID - 1];
  
  memcpy_P(&temp, tempref, sizeof(chordShape));
  
  return temp;
}
