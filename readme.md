# ASETNIOP Chorded Keyboard

#### Description:
A keyboard concept and layout designed by Zach Dennis in 2012, based on the function and setup of the virtual keyboard on his [website](https://www.asetniop.com). Its goal is to simplify typing on tablets and similar mobile devices by assigning each letter of the alphabet to a chorded pattern on the layout, where each finger has 1, and only 1 key to press. It operates similarly to a steno keyboard for stenography.

My design is a hardware-based implementation of the layout for interfacing with a PC. Existing hardware based solutions have been created in the past by other individuals utilizing libraries and tools for keyboard making, but I wanted to challenge myself to create a similar design from scratch, while improving my proficiency at programming microcontrollers, and possibly refine certain details of the layout. I was determined throughout the project to make it fit onto an ATMega 328p/32u4 with its 2kB of sram and 32kB of flash.


![image](https://user-images.githubusercontent.com/33296248/147901579-bd6e3145-adde-4905-bbc8-30b6152ae106.png)


#### Design Considerations
1. Design must work as a standard HID peripheral with no additional drivers. In effect, it should be plug-and-play. This almost instantly excludes certain microcontrollers that don't come equipped with hardware USB peripheral abilities. Including my original prototype board, the atmega328p used in the Arduino Uno.
2. Due to the nature of the keyboard layout, it will be ultimately setup as a split keyboard, connected by a sort of cable between the two modules. My intention is to use a headphone cable to transmit the I^2C protocol, with the right keyboard sending telemetry data back to the left side upon request to be processed.
3. Final design should be ergonomic and have optional risers of some kind, same as a normal keyboard.



#### Potential Improvements:
Though the project is technically in a working state as-is, if I work on this project in the future I may consider adding:
* some sort of LED setup to simplify keyboard operation
* More ergonomic layout, hand raisers. potentially a 3d printed frame with kailh choc switches
* Navigation mode, arrow keys, pgup, pgdown, media controls
* Meta mode? Ability to hold ctrl, shift, alt, windows then press other key combos. May not be needed, this is more of a typing keyboard than anything but would be interesting.
* 11th and 12th switches for the thumbs - this kind of goes against the philosophy of the keyboard layout (only 1 key for every finger), but it adds additional functionality without requiring crazy chords to be entered in its place, and expands functionality. Maybe as a variation on the project.



#### Necessary Equipment
Though I was able to get working code that I'm happy with, in my spare time I intend to create a more fleshed out design using a 3d printer.
The specific supplies required for this project as of late are: 
* Adafruit Itsy Bitsy 32u4 5v 
* Currently nonexistant 3d-printed shell to house components
* Keyboard switches - 10, 12?
* Keyboard caps - 10, 12?



#### Project Complications (Ordered by resolve order/ To resolve next order)
The ASETNIOP layout is fairly consistent in its function, but there are some discrepencies that need to be accounted for. They're listed in the order I intend to (or have) completed them.

1. Initally, I had a hard time finding a reference for the chordmapping on the keyboard, especially one that I could easily convert into a format C++ could understand. I ended up using the JSON file from the asetniop creator's [github page](https://github.com/asetniop/Predictive), then creating a python file to transfer the data into a .ino file that Arduino's IDE can understand.

2. The ATMEGA 328p only has 2kb of SRAM, which acts as the program space in atmel's SOC processors. All constant strings are moved from flash to SRAM at startup. However, my lookup table would at minimum take up nearly 6-9kB, which posed a problem. To solve this, I instead told the processor to keep the hashmap in flash using PROGMEM, and used memcpy_p() from <pgmspace.h> to fetch data as needed. This required changing the layout of the struct to include an array of chars rather than a reference to a string, vastly increasing the size of the hash table. Getting around this would be messy and potentially slower. This approach did end up working.

3.  When a word or partial is picked based on the user's input, the possibility exists that there is no such word or partial in the struct.  In the case of an event like this, there is an order of precedence that the program will use to select the next best chord or partial. The 4 primaries are:
    * left partial
    * right partial
    * left word
    * right word 

    The specific order follows a distinct pattern, shared across all 4 primaries:
    - For a partial: primary, opposing partial, congruent word, opposing partial
    - For a word: primary, opposing word, congruent partial, opposing partial

    I discovered that this behavior exhibited by the order of chord priority works similar to a binary counter. If you assign each partial and word a number from 0 to 4 in binary (lp:00, rp:01, lw:10, rw:11) and xor this with indices from a for loop, giving the next chord in the sequence. For instance, the right partial's order would be: 01, 00, 11, 10:
    * 0b01 ^ 0 = 0b01
    * 0b01 ^ 1 = 0b00
    * 0b01 ^ 2 = 0b11
    * 0b01 ^ 3 = 0b10

4. The ATMEGA 328P does not support USB peripheral mode, so development was switched to the ATMEGA32u4, due to its onboard support for USB Peripheral. The exact breakout board in use is the Itsy Bitsy 32U4 by Adafruit. The flash and SRAM of this device is identical to the 328P but includes these additional features which were needed in this design.

5. The shift key with this layout posed a particular problem. It cycles through many modes, and creating a compact transition did take me a bit to figure out after staring at a paper for a while. Same as above I am able to assign each mode a numeric value, and modify the chord as necessary based on the input.

6. Backspace has special requirements, though it's activated like a normal chord, which posed a challenge deciding the best way to implement it. It's activated by holding the 't' and 'p' keys at the same time (right finger on both hands). As long as you're holding both keys, i've decided it should hold the keypress so the OS can repeat the keypress as necessary. Though this is not the perfect solution as there is a chance that backspace will momentarily be pressed when typing chords composed of the same keys that backspace uses.

7. The keyboard is designed to be split in two halves. A 4 pin headphone jack would be used to send power and connect the main keyboard to the secondary right-hand one, transmitting I2C between the two halves. There are 1-wire protocols but I^2C was the most practical and accessible for this application.
