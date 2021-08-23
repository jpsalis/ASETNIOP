#include "lookup.h"




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
