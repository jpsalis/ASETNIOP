#ifndef LOOKUP_H
#define LOOKUP_H

#include <ctype.h>

// SIZE DEFINITIONS
#define TABLESIZE    255
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

chordShape getData(uint8_t shape);

const chordShape chordLookup[TABLESIZE] PROGMEM = {
{{'a', 'A'}},
{{'s', 'S'}},
{{'w', 'W'}},
{{'e', 'E'}},
{{'x', 'X'}},
{{'d', 'D'}},
{.dict = {"ad", "", "we", ""}},
{{'t', 'T'}},
{{'f', 'F'}},
{{'c', 'C'}},
{.dict = {"ca", "", "fact", ""}},
{{'r', 'R'}},
{.dict = {"ar", "", "are", ""}},
{.dict = {"ce", "", "set", ""}},
{.dict = {"wer", "", "were", ""}},
{{'n', 'N'}},
{{'q', 'Q'}},
{{'j', 'J'}},
{.dict = {"wn", "ja", "san", "jaw"}},
{{'y', 'Y'}},
{.dict = {"ay", "yea", "any", "nay"}},
{.dict = {"end", "nd", "eyes", "need"}},
{.dict = {"and", "new", "and", "new"}},
{{'b', 'B'}},
{.dict = {"ab", "nat", "fan", ""}},
{.dict = {"bs", "nc", "bc", ""}},
{.dict = {"can", "nsta", "can", ""}},
{.dict = {"be", "nte", "be", "yet"}},
{.dict = {"ran", "year", "ran", "next"}},
{.dict = {"enc", "nce", "best", ""}},
{.dict = {"", "", "went", "years"}},
{{'i', 'I'}},
{{'z', 'Z'}},
{{'k', 'K'}},
{.dict = {"wi", "ka", "ask", "kaa"}},
{{',', '<'}},
{.dict = {"ze", "ize", "xiii", "ix"}},
{.dict = {"di", "id", "did", "kids"}},
{.dict = {"ake", "idea", "said", "idea"}},
{{'v', 'V'}},
{.dict = {"fi", "if", "fit", "if"}},
{.dict = {"ck", "ic", "sit", "its"}},
{.dict = {"wit", "ica", "wait", "isaac"}},
{.dict = {"ve", "ir", "ever", "ive"}},
{.dict = {"ave", "ife", "five", ""}},
{.dict = {"rk", "ice", "tried", ""}},
{.dict = {"", "", "first", ""}},
{{'h', 'H'}},
{.dict = {"ain", "ha", "ah", "han"}},
{.dict = {"sh", "his", "skin", "his"}},
{.dict = {"wh", "has", "wish", "has"}},
{.dict = {"ein", "he", "eh", "he"}},
{.dict = {"aine", "hea", "annie", "yeah"}},
{.dict = {"she", "ind", "she", "kind"}},
{.dict = {"", "", "when", "had"}},
{.dict = {"th", "ith", "bit", "hit"}},
{.dict = {"tha", "hat", "that", "hat"}},
{.dict = {"ch", "ich", "this", "nick"}},
{.dict = {"", "", "with", "jack"}},
{.dict = {"the", "her", "the", "her"}},
{.dict = {"", "", "father", "have"}},
{.dict = {"", "", "these", "interest"}},
{.dict = {"", "", "where", "heard"}},
{{'o', 'O'}},
{{'(', '['}},
{{'.', '>'}},
{.dict = {"wo", "ow", "wow", ""}},
{{'-', '_'}},
{.dict = {"xo", "ox", "", ""}},
{.dict = {"do", "od", "do", "odd"}},
{.dict = {"dow", "owe", "wood", "owed"}},
{{'g', 'G'}},
{.dict = {"fo", "of", "ago", "of"}},
{.dict = {"co", "oc", "cost", ""}},
{.dict = {"two", "oac", "two", ""}},
{.dict = {"ro", "or", "get", "or"}},
{.dict = {"for", "orta", "for", "offer"}},
{.dict = {"eco", "ord", "good", "order"}},
{.dict = {"", "", "words", "offered"}},
{{'u', 'U'}},
{.dict = {"au", "ua", "", ""}},
{.dict = {"su", "us", "soon", "us"}},
{.dict = {"wou", "now", "won", "now"}},
{.dict = {"eno", "yo", "", "you"}},
{.dict = {"que", "oya", "anyone", ""}},
{.dict = {"du", "und", "done", "use"}},
{.dict = {"", "", "down", "owned"}},
{.dict = {"bu", "ng", "but", "not"}},
{.dict = {"abo", "uf", "about", ""}},
{.dict = {"con", "uc", "cut", "just"}},
{.dict = {"", "", "town", ""}},
{.dict = {"rou", "ur", "turn", "your"}},
{.dict = {"", "", "before", "often"}},
{.dict = {"", "", "course", "once"}},
{.dict = {"", "", "because", "understand"}},
{{'l', 'L'}},
{.dict = {"al", "la", "all", "la"}},
{.dict = {"sio", "ok", "skills", "look"}},
{.dict = {"wil", "low", "will", "low"}},
{.dict = {"el", "le", "el", "lie"}},
{.dict = {"eal", "lea", "alex", ""}},
{.dict = {"sel", "ld", "else", "like"}},
{.dict = {"", "", "well", "lead"}},
{.dict = {"tio", "ig", "till", "lot"}},
{.dict = {"atio", "lf", "fall", ""}},
{.dict = {"cl", "lic", "still", "lost"}},
{.dict = {"", "", "call", "last"}},
{.dict = {"rl", "ove", "tell", "over"}},
{.dict = {"", "", "felt", "life"}},
{.dict = {"", "", "told", "lives"}},
{.dict = {"", "", "work", "least"}},
{.dict = {"", "ho", "", "oh"}},
{.dict = {"qui", "lan", "annual", "hall"}},
{.dict = {"sho", "kno", "soul", "john"}},
{.dict = {"", "", "who", "how"}},
{.dict = {"ely", "ly", "", "only"}},
{.dict = {"", "", "alone", "lay"}},
{.dict = {"", "", "should", "house"}},
{.dict = {"", "", "would", "usually"}},
{.dict = {"gh", "ing", "thought", "into"}},
{.dict = {"", "", "again", "having"}},
{.dict = {"", "", "such", "looking"}},
{.dict = {"", "", "without", ""}},
{.dict = {"", "", "through", "other"}},
{.dict = {"", "", "another", ""}},
{.dict = {"", "", "could", "others"}},
{.dict = {"", "", "already", "however"}},
{{'p', 'P'}},
{{'?', '/'}},
{{')', ']'}},
{.dict = {"spa", "pas", "sap", "pass"}},
{{'\'', '"'}},
{.dict = {"xp", "pea", "", ""}},
{.dict = {"spe", "'s", "deep", "'s"}},
{.dict = {"we'", "'w", "we'd", "passed"}},
{.dict = {"tp", "pt", "", ""}},
{.dict = {"apt", "pat", "tap", "pat"}},
{.dict = {"", "pts", "", ""}},
{.dict = {"cap", "pac", "cap", "past"}},
{.dict = {"t'", "pr", "", "per"}},
{.dict = {"at'", "par", "appear", "part"}},
{.dict = {"t's", "pres", "step", "press"}},
{.dict = {"", "", "we're", "perfect"}},
{{'m', 'M'}},
{.dict = {"am", "ma", "am", "man"}},
{.dict = {"sm", "ms", "", "ms"}},
{.dict = {"sam", "mas", "sam", "mass"}},
{.dict = {"em", "me", "enemy", "my"}},
{.dict = {"ame", "may", "an'", "many"}},
{.dict = {"dn'", "med", "seemed", "jem"}},
{.dict = {"", "", "same", "made"}},
{.dict = {"tm", "mb", "", ""}},
{.dict = {"fam", "mat", "", "mba"}},
{.dict = {"stm", "mst", "", ""}},
{.dict = {"", "", "camp", "pants"}},
{.dict = {"rm", "n't", "remember", "mr"}},
{.dict = {"", "", "army", "matter"}},
{.dict = {"", "", "spent", "mrs"}},
{.dict = {"", "", "came", "parents"}},
{{'!', '!'}},
{.dict = {"appi", "pai", "", "pizza"}},
{.dict = {"spi", "ips", "sip", "piss"}},
{.dict = {"spai", "isap", "", ""}},
{.dict = {"epi", "i'", "", "'i"}},
{.dict = {"epai", "iz'", "", ""}},
{.dict = {"disp", "keep", "spies", "keep"}},
{.dict = {"", "", "speak", "paid"}},
{.dict = {"tip", "pti", "tip", "pit"}},
{.dict = {"apit", "pati", "", ""}},
{.dict = {"cip", "pic", "tips", "pick"}},
{.dict = {"", "", "", "pack"}},
{.dict = {"rip", "pri", "trip", "i've"}},
{.dict = {"", "", "repair", "private"}},
{.dict = {"", "", "despite", "it's"}},
{.dict = {"", "", "we've", "paris"}},
{.dict = {"", "im", "", "him"}},
{.dict = {"ami", "hap", "aim", "pain"}},
{.dict = {"ship", "mis", "ship", "miss"}},
{.dict = {"", "", "spanish", "mask"}},
{.dict = {"emi", "ime", "", "i'm"}},
{.dict = {"", "", "examine", "happy"}},
{.dict = {"", "", "she'd", "mind"}},
{.dict = {"", "", "shame", "make"}},
{.dict = {"tim", "mit", "tim", "ibm"}},
{.dict = {"", "", "", "path"}},
{.dict = {"", "", "smith", "pitch"}},
{.dict = {"", "", "captain", "match"}},
{.dict = {"", "", "them", "prevent"}},
{.dict = {"", "", "remain", "haven't"}},
{.dict = {"", "", "didn't", "isn't"}},
{.dict = {"", "", "that's", "perhaps"}},
{{';', ':'}},
{.dict = {"apo", "opa", "", ""}},
{.dict = {"spo", "pos", "", ""}},
{.dict = {"soap", "pow", "soap", ""}},
{.dict = {"eop", "peo", "", "pope"}},
{.dict = {"eapo", "opea", "", ""}},
{.dict = {"espo", "pose", "", "possessed"}},
{.dict = {"", "", "exposed", ""}},
{.dict = {"top", "pot", "top", "pot"}},
{.dict = {"gap", "pag", "gap", "potato"}},
{.dict = {"stop", "post", "stop", "post"}},
{.dict = {"", "", "gaps", ""}},
{.dict = {"rop", "pro", "report", "poor"}},
{.dict = {"", "", "rapport", "page"}},
{.dict = {"", "", "stopped", "process"}},
{.dict = {"", "", "corporate", "power"}},
{.dict = {"", "om", "", "up"}},
{.dict = {"amo", "oma", "", "mao"}},
{.dict = {"som", "mos", "sum", "jump"}},
{.dict = {"", "", "woman", "mason"}},
{.dict = {"emo", "ome", "", "money"}},
{.dict = {"", "", "", ""}},
{.dict = {"", "", "some", "opened"}},
{.dict = {"", "", "women", "paused"}},
{.dict = {"tom", "mot", "tom", "put"}},
{.dict = {"", "", "among", "obama"}},
{.dict = {"", "", "common", "most"}},
{.dict = {"", "", "famous", "moscow"}},
{.dict = {"", "", "room", "more"}},
{.dict = {"", "", "from", "program"}},
{.dict = {"", "", "don't", "person"}},
{.dict = {"", "", "company", "major"}},
{.dict = {"", "pl", "", "pool"}},
{.dict = {"appl", "pla", "apollo", "lap"}},
{.dict = {"spok", "posi", "slip", "lips"}},
{.dict = {"", "", "slap", "pillow"}},
{.dict = {"eopl", "ple", "", "people"}},
{.dict = {"", "", "apple", "pale"}},
{.dict = {"", "", "spoke", "people's"}},
{.dict = {"", "", "we'll", "please"}},
{.dict = {"topi", "ptio", "", "pilot"}},
{.dict = {"", "", "flip", "plato"}},
{.dict = {"", "", "split", "politics"}},
{.dict = {"", "", "capital", "political"}},
{.dict = {"", "", "grip", "prove"}},
{.dict = {"", "", "april", "plate"}},
{.dict = {"", "", "replied", "police"}},
{.dict = {"", "", "special", "place"}},
{.dict = {"", "mil", "", "million"}},
{.dict = {"", "", "animal", "human"}},
{.dict = {"", "", "shop", "mission"}},
{.dict = {"", "", "small", "plans"}},
{.dict = {"", "", "employee", "home"}},
{.dict = {"", "", "example", "play"}},
{.dict = {"", "", "simply", "pulled"}},
{.dict = {"", "", "explained", "played"}},
{.dict = {"", "", "", "might"}},
{.dict = {"", "", "film", "imagination"}},
{.dict = {"", "", "coming", "much"}},
{.dict = {"", "", "almost", "making"}},
{.dict = {"", "", "government", "mother"}},
{.dict = {"", "", "family", "important"}},
{.dict = {"", "", "something", "possible"}},
{.dict = {"", "", "especially", "himself"}}
};

#endif
