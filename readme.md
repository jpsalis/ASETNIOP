# ASETNIOP
A keyboard concept and layout designed by Zach Dennis in 2012. Its goal is to simplify typing on tablets and similar mobile devices by assigning each letter of the alphabet to a chorded pattern on the layout, where each finger has 1, and only 1 key to press. 

My design is a hardware-based implementation of the layout. Existing hardware based solutions have been created in the past by other individuals, but I wanted to challenge myself to create a similar creation from scratch, while improving my proficiency at programming microcontrollers, and possibly refine certain details of the layout while i'm at it.

## Design Considerations
1. Design must work as a standard HID peripheral with no additional drivers. In effect, it should be plug-and-play. This almost instantly excludes certain microcontrollers that don't come equipped with hardware USB peripheral abilities.
2. Due to the nature of the keyboard layout, it will be ultimately setup as a split keyboard, connected by a sort of cable between the two modules. My intention is to use a headphone cable to transmit the I2C protocol, with the right keyboard sending telemetry data back to the left side upon request to be processed.


## Potential Improvements:
Once the keyboard is configured, I can go about adding some more functionality. In particular, I've been considering adding:
* some sort of LED setup to simplify keyboard operation
* More ergonomic layout, hand raisers. potentially a 3d printed frame with kailh choc switches
* Navigation mode, arrow keys, pgup, pgdown, media controls
* Meta mode? Ability to hold ctrl, shift, alt, windows then press other key combos. May not be needed, this is more of a typing keyboard than anything but considering.
* 11th and 12th switches for the thumbs - this kind of goes against the philosophy of the keyboard layout (only 1 key for every finger), but it adds additional functionality without requiring crazy chords to be entered in its place.



## Necessary Equipment
Later down the line, I intend to create a more fleshed out prototype. The supplies required for this project as of late are:
* Keyboard switches - 10
* keyboard caps - 10, 12?
* ATMEGA32U4 Dev Board (Testing commenced with 328p (Ardu. Uno), lacked usb peripheral mode)
* Soldering iron, flux, solder (owned)
* Hot Air Rework Station? (If I decide to go beyond a dev board for the processor)





## Initial complications: 
The ASETNIOP layout is fairly consistent in its function, but there are some discrepencies that need to be accounted for. They're listed in the order I intend to (or have) completed them.

1. Initally, I had a hard time finding a reference for the chordmapping on the keyboard, especially one that I could easily convert into a format C could understand. I ended up using the JSON file from the asetniop creator's website, https://asetniop.com.

2. The ATMEGA 328p only has 2kb of SRAM, which is pretty much the equivallent of program space in atmel's SOC processors. Considering my lookup table would at minimum take up nearly 6-9kB, this wasn't going to work. To solve this, I instead kept the constant struct array in the onboard flash storage using PROGMEM, and used memcpy_p() from <pgmspace.h> to fetch data as needed. 

3.  When a word or partial is picked based on the user's input, the possibility exists that there is no such word or partial in the struct.  In the case of an event like this, there is an order of precedence that the program will use to select the next best chord or partial. The 4 primaries are:
    * left partial
    * right partial
    * left word
    * right word 

    The specific order follows a distinct pattern, shared across all 4 primaries:
    - For a partial: primary, opposing partial, congruent word, opposing partial
    - For a word: same side word, opposing word, congruent partial, opposing partial

    With a bit of luck, I was able to discover that this behavior exhibited by the order of chord priority is very similar to a binary counter. If you assign each partial and word a number from 0 to 4 in binary (lp:00, rp:01, lw:10, rw:11) and xor this with indices from a for loop, you will then get the next chord in the sequence. For instance, the right partial's order would be: 01, 00, 11, 10:
    * 1 ^ 0 = 0b01
    * 1 ^ 1 = 0b00
    * 1 ^ 2 = 0b11
    * 1 ^ 3 = 0b10

4. UNIMPLEMENTED: The shift key with this layout poses a particular problem. It cycles through many modes, and creating a compact transition between them is especially challenging. However, same as above I am able to assign each mode a numeric value and with a bit of work, diagram and setup a control flow for using a switch statement.

5. UNIMPLEMENTED: Backspace doesn't work like a normal chord. It's activated by holding the 't' and 'p' keys at the same time. As long as you're holding both keys, it will hold the keypress so the OS can use repeat mode as necessary. Because the key is normally not pressed until all keys are released, I can't see any way to implement it except to make the repeat a feature of the keyboard itself.

6. UNRESOLVED: The ATMEGA 328P does not support USB peripheral mode, once I get out of the initial testing phase with serial port I have to port to some other processor that does, so my keyboard can natively be supported no matter what device I plug it into. I am considering the ATmega32u4.
