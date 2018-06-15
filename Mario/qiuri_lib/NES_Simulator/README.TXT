REVISION HISTORY
----------------

VERSION 1.

I'd like to contribute something to the cause. The original 6502 emulator 
that I used in EMU (which has long since been retired) is attached to 
this email message. I don't know where I got it originally, but I tweaked 
it to run faster than the original. It's here so people can get a good 
start or see what's going on in emulation. Please include the following 
stipulations in your FAQ if you decide to put it in there:

* I am not the original author of the code. I got it from somewhere off the
  net, and remember reading that it was OK to distribute. If someone knows
  who the original author is, please email me or include it. I made some
  modifications to it.
* I did some quickie optimizations on it
* EMU Does not use any of this code
* It's nowhere near optimized 100%
* Memory reads don't use the get6502memory call (made it lots faster - 3X)
* put6502memory could be rewritten as a macro to increase speed
* CPU Modes could be rewritten as macros to prevent a call dereference
* There are some problems in the BCD emulation in the ABC/SBC instructions

All in all, it runs at about 2.5-3MHZ emulated on a 486/100 when all 
stack probes are removed. This code was my starting block when I began 
coding the Asteroids emulator back in February.

I hope people find this useful. This code will run Asteroids & Asteroids 
Deluxe unmodified. The vector generator part isn't included...

Anything else I can add to the FAQ to assist? Let me know!

-->Neil


VERSION 2.

Version 1 came in the form of a text file. This version is in the form
of a .c and a .h file.

Any occurances of >> (C bit shift operator) in version 1 were corrupt
and converted to a single > (greater than comparison). This is now fixed.

When exec6502() is called the number of machine cycles to execute is now
passed as opposed to the number of instructions. This should make interrupt
timing more accurate.

