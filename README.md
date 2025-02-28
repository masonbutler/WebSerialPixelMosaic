# WebSerialPixelMosaic
I learned about WebSerial and I thought it'd be cool to do something with it that involved LEDs. I used a cheap 8x8 LED matrix (but theoretically, it could be any size). Uses USB CDC to communicate with a webapp (written in HTML, CSS, and Javascript). I'm not a web dev so if I did something bad in Javascript, I'm sorry :(  I just like trying out new cool things as I learn about them.

NOTE: This may not work in some browsers (depending on security settings), but it worked fine in Chrome (Version 133.0.6943.142 (Official Build) 64-bit) and Microsoft Edge (Version 133.0.3065.82 (Official build) 64-bit).

The LED matrix looks like this (I made and 3D printed an LED diffuser, but I'm working on modifying it to stop some of the light bleed):

<img width="453" alt="image" src="https://github.com/user-attachments/assets/a22a26c8-a45a-4abb-ba20-cacaf9e8d0c7" />


## How does it work?
Just visit the webpage, connect the USB device, then press the connect button on the webpage.

<img width="419" alt="image" src="https://github.com/user-attachments/assets/1239391d-ac75-48ed-99f5-8863f9e32a79" />


## Technical Stuff
The project uses an ATSAM L21 Explained Pro Devboard. The LED data is converted to a binary stream to provide the correct data format for the WS2812B LEDs over SPI (at 3 MHz). Hence the reason why there are separate commands to update the LED data for an individual LED or all at once. If you sent 1 LED, you just want to update the single LED bitstream then update the LED display. But if you send a whole image at once, you'll want to update all the LEDs at once after you received all the LED data.

## Physical Connections
PA16: LED Data Output (SPI MOSI)
(And the matrix has to be connected to any VCC (3V3) and GND rail.)

## Compiling

Download Microchip Studio (Atmel Studio) 7.0.2594 then open project file from repo

## Bugs
Sometimes the transmission of a whole image gets messed up on the device when sending over USB. Typically, certain "pixels" are missed on the LED matrix, or they have the slightly wrong color, I'm working on figuring this out.
The LED diffuser I made and 3D printed needs to do a bit better at stopping some of the light bleed.

