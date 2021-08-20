import json
import sys

DEF = ""
lookup = [('lw', 'tlw'), ('rw', 'trw'), ('lp', 'tlp'), ('rp', 'trp')]

def main():
    with open('asetniop.txt') as f:
        data = f.read()

    js = json.loads(data)

    output = []

    for key in js:
        temp = js[key]
        index = int(key)
        output.insert(index, {})
        # FINAL GOAL: 
        for pair in lookup:
            if index == 0:
                continue
            output[index][pair[0]] = temp.get(pair[1])[0] if pair[1] in js[key].keys() else "";
            

    for i in range(len(output)):
        print(f"{i:08b}: {output[i]}")

    return 0


def swap(

if __name__ == "__main__":
    main()
