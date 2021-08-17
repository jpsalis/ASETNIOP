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
        
        output.insert(x, {})
        for pair in lookup:
            
            output[x][pair[1]] = temp.get(pair[0])[0] if pair[0] in js[key].keys() else "";
            

    for item in output:
        print(item)

    return 0




if __name__ == "__main__":
    main()
