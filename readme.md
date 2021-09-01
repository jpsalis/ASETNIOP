# ASETNIOP
A keyboard concept and layout designed by Zach Dennis in 2012. Its goal is to simplify typing on tablets and similar mobile devices by assigning each letter of the alphabet to a chorded pattern on the layout, where each finger has 1, and only 1 key to press. 

My design is a hardware-based implementation of the layout. Existing hardware based solutions have been created in the past by other individuals, but I wanted to challenge myself to create a similar creation from scratch, while improving my proficiency at programming microcontrollers, and possibly refine certain details of the layout while i'm at it.

## Design Considerations
1. Design must work as a standard HID peripheral with no additional drivers. In effect, it should be plug-and-play. This almost instantly excludes certain microcontrollers that don't come equipped with hardware USB peripheral abilities.
2. Due to the nature of the keyboard layout, it will be ultimately setup as a split keyboard, connected by a sort of cable between the two modules. My intention is to use a headphone cable and transmit data with the I2C protocol, with the right-side of the keyboard sending telemetry data back to the left side upon request to be processed.



## Necessary Equipment
Later down the line, I intend to create a more fleshed out prototype. Regardless of the route I take, I need to acquire:
* ATMEGA32U4 Dev Board (Testing commenced with 328p (Ardu. Uno), lacked usb peripheral mode)
* Soldering iron, flux, solder (owned)
* Hot Air Rework Station? (If I decide to go beyond a dev board for the processor)




## Initial complications: 
The ASETNIOP layout is fairly consistent in how it works, but there are some discrepencies that need to be accounted for. They're listed in the order I intend to (or have) completed them.

1. I initally had a hard time finding a reference for the layout of the keyboard, especially one that I could pull data from. Luckily, the asetniop website uses a .json file behind the scenes of the website keyboard to store this information. With a bit of effort, it's possible to write this to a new file with python so C can understand it.

2. Backspace doesn't work like a normal chord. It's activated by holding the 't' and 'p' at the same time. As long as you're holding both keys, it will hold the keypress so the OS can use repeat mode as necessary. Because the chord is normally not pressed until all keys are released, My intention is to implement this on the keyboard itself, as some sort of function working on delta time or using an interrupt.

3. How do you dictate the priority of 1 chord over another (if no chord exists)? Priority for a chord goes towards the side of the keyboard it originated from, but I need a way to determine which side is selected next that isn't a massive pile of hot garbage.

4. The ATMEGA 328P does not support USB peripheral mode, once I get out of the initial testing phase with serial port I have to port to some other processor that does, so my keyboard can natively be supported no matter what device I plug it into. I am considering the ATmega32u4 because it includes this hardware USB peripheral support.
