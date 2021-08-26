import json
import sys

DEF = ""
lookup = [('base', 'base'), ('baseshift', 'baseshift'),('lp', 'tlp'), ('rp', 'trp'), ('lw', 'tlw'), ('rw', 'trw')]


# FINAL GOAL: Array of 255 elements, each storing data about that keypress.
# IMPORTANT: B/C 0b0000.0000 serves no purpose, the array can be shifted 1 element, so 0000.0000 becomes 0000.0001, so on.
def main():
    with open('asetniop.txt') as f:
        data = f.read()

    data = json.loads(data)

    output = []
    max_len = {}
    for key in data:
        index = int(key)
        if index == 0:
            continue

        # Insert a new row for new data to be entered.
        output.insert(index - 1, {})

        # Loop through selections in lookup match table. Append to output if it's in the original table
        for pair in lookup:
            if pair[1] in data[key].keys():
                word = data[key].get(pair[1])[0]
                
                output[index - 1][pair[0]] = word
                
                # Find longest of each lookup pair, set length if longer or doesn't exist yet.
                if pair[0] not in max_len or max_len[pair[0]] < len(word):
                    max_len[pair[0]] = len(word)

                
                        
        print(f"{index:08b}: {output[index-1]}")

    print(max_len)
    return 0
        


if __name__ == "__main__":
    main()
