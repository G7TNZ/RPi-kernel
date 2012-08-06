RPi-bare
========

Raspberry Pi bare metal experimentation

## So far it includes: 
1. Methods to change the GPIO pins state.
2. Methods to flash the Status Led in Morse code. This is useful for debugging without a screen.
3. Methods to convert a variable into a decimal or hex string that can then be  
     flashed in Morse on the status Led.
4. Initialisation of the framebuffer. Works in 640x480, 1024x768, 1280x1024 all in 24bit.
5. Methods to change the foreground and background colours.
6. A character set that can be written to the screen.
7. Method to dump a block of memory in hex to the screen.
8. GPIO7 setup for Morse key input.

## Known issues: 
The framebuffer can be tempermental. Sometimes it gives a black screen, sometimes  
a screen cleared to the colour chosen, and mostly it works with a screen with text  
written. Every time it gets to the termination where GPIO7 is refelcted in the status LED. (Or where the status LED flashes a string of Morse letter 'V' depending on which is active.)

## Intentions:
Enable other screen resolutions: 16 and 32 bit.  
Eventually to get keyboard input via the USB port and ethernet functioning.  
In the short term to put a Morse key onto a GPIO pin for input and write a Forth  
interpreter.
Long term: USB, ethernet and SD card read/write. 

## Setup:
Make a partition on an SD card. One way to do this is to use a card that has been  
installed with a distribution of Linux for the RPi and just overwrite the contents  
of the boot partition. You do not need the card to have a high capacity as the files
you need to install come to less than 3Mb. 

You can find instructions as to where to find the right files and what to install  
here:  
        http://www.bassetlug.org.uk/pi_articles

After building; the files that are in the SDcard folder need to be written to the SDcard.

My thanks go to many (particularly dwelch67, tufty and Cycl0ne) on the Raspberry Pi forum who have outlined the crucial first steps:

 http://www.raspberrypi.org/phpBB3/viewforum.php?f=72

Martin Wells  
Ischus  
www.ischus.com

