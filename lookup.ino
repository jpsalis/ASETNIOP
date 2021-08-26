#include "lookup.h"


//TODO: get table to work with progmem. Currently able to access data but takes up w a a a y too much space.
const chordShape chordLookup[TABLESIZE] PROGMEM = {
  {{'a', 'A'}},
  {{'s', 'S'}},
  {{'w', 'W'}},
  {{'e', 'E'}},
  {{'x', 'X'}},
  {{'d', 'D'}},
  {.dict = {"ad" , "", "we", ""}},
  {{'t', 'T'}},
  {{'f', 'F'}},
  {{'c', 'C'}},
  {.dict = {"ca" , "", "fact" , ""}},
  {{'r', 'R'}},
  {.dict = {"ar" , "", "are"  , ""}}, 
  {.dict = {"ce" , "", "set"  , ""}},
  {.dict = {"wer", "", "were", ""}}
};


chordShape getChord(uint8_t shape) {
  chordShape temp;
  memcpy_P(&temp, &chordLookup[shape - 1], sizeof(chordShape));
  return temp;
}
