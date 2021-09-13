import json
import sys

DEF = ""
lookup = [('base', 'base'), ('baseshift', 'baseshift'),('lp', 'tlp'), ('rp', 'trp'), ('lw', 'tlw'), ('rw', 'trw')]
text = {
    "header" : "const chordShape chordLookup[TABLESIZE] PROGMEM = {",
    "footer" : "};"
}


# FINAL GOAL: Array of 255 elements, each storing data about that keypress.
# IMPORTANT: B/C 0b0000.0000 serves no purpose, the array can be shifted 1 element, so 0000.0000 becomes 0000.0001, so on.
def main():
    with open('asetniop.txt') as f:
        data = json.loads(f.read())
    print(text["header"])
    temp = {}
    #max_len = {}
    
    for key in data:
        key = int(key)

        # skips initial item
        if key == 0:
            continue
        
        # print comma to the line if and only if you've passed the first loop
        if key > 1:
            print(",")
        
        # prepare dict for new input
        temp.clear()

        # Loop through selections in lookup match table. Append to output if it's in the original table
        for pair in lookup:
            if pair[1] in data[str(key)].keys():
                word = data[str(key)].get(pair[1])[0]
                
                temp[pair[0]] = word

        # print(f"{key:08b}: ", end="") DEBUG: Prints binary equivallent

        # Check which version of the text to print, print appropriately
        if ('base' in temp and 'baseshift' in temp):
            print("{{'" + 
                temp.get('base') + "', '" + 
                temp.get('baseshift') + 
                "'}}",
            end = ""
            )

        else:
            print('{.dict = {"' + 
                temp.get('lp', '') + '", "' + 
                temp.get('rp', '') + '", "' +
                temp.get('lw', '') + '", "' +
                temp.get('rw', "") + 
                '"}}', 
            end = ""
            )

    print(f'\n{text["footer"]}')
    return 0
        
if __name__ == "__main__":
    main()
