RPi-bare
========

Raspberry Pi bare metal experimentation

So far it includes:
1. Methods to change the GPIO pins state.
2. Methods to flash the Status Led in Morse code. This is useful for debugging.
3. Methods to convert a variable into a decimal or hex string that can then be
     flashed in Morse on the statuis Led.
4. Initialisation of the framebuffer. Works in 640 x 480 x 24bit
5. Methods to change the foreground and background colours.
6. A character set than can be written to the screen.

Known issues:
The framebuffer can be tempermental. Sometimes it gives a black screen, sometimes
a screen cleared to the colour chosen, and mostly it works with a screen with text
written. Every time it gets to the termination where the status Led flashes a string
of Morse letter 'V'.

Intentions:
Enable other screen modes: 1024x768 etc.
Eventually to get keyboard input via the USB port and ethernet functioning.
In the short term to put a Morse key onto a GPIO pin for input and write a Forth
interpreter. 

Setup before starting:
Make a partition on an SD card. You do not need the card to have a high capacity as
the files you need to install come to less than 3Mb. 

You can find instructions as to where to find the right files and what to install
here:
        http://www.bassetlug.org.uk/pi_articles

that describes how to use the Rpi itself to sdo the compilation, but it is better to use qemu, scratchbox2 with a toolchain, which is how this project is developed.

To compile the project using the Makefile you need to add a 'bin' folder in the same
place as the Makefile. After building; the files that are in the SDcard folder need
to be written to the SDcard.


Martin Wells
Ischus
www.ischus.co.uk
