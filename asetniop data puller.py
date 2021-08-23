import json
import sys

DEF = ""
lookup = [('base', 'base'), ('baseshift', 'baseshift'),('lp', 'tlp'), ('rp', 'trp')]


# FINAL GOAL: Array of 255 elements, each storing data about that keypress.
# IMPORTANT: B/C 0b0000.0000 serves no purpose, the array can be shifted 1 element, so 0000.0000 becomes 0000.0001, so on.
def main():
    with open('asetniop.txt') as f:
        data = f.read()

    js = json.loads(data)

    output = []

    for key in js:
        index = int(key)
        if index == 0:
            continue
            
        temp = js[key]
        output.insert(index - 1, {})

        
        
        for pair in lookup:
            output[0]['base'] = js['1'].get('base')[0] 



            #output[index - 1][pair[0]] = temp.get(pair[1])[0] if pair[1] in js[key].keys() else ""
            if pair[1] in js[key].keys():
                output[index - 1][pair[0]] = temp.get(pair[1])[0]
                        
        print(f"{index:08b}: {output[index-1]}")

    return 0



if __name__ == "__main__":
    main()
