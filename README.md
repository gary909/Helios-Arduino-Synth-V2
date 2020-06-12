# Helios-Arduino-Synth-V2


****************************************************************************************************
Part 2 of a simple Arduino Synth tutorial
****************************************************************************************************


It's been a while coming, but thanks to lock-down I've finally managed to finish part 2 of my simple Arduino synth, the impressively titled 'Helios-One'.  Don't expect it to sound like it's name.  Although maybe, a bit like lock-down, perhaps it sounds 'sick'.  

Here's part 1 of the tutorial in case you missed it; 

https://bloghoskins.blogspot.com/2019/07/helios-one-arduino-synth-part-1.html

In that part we set up an Arduino to work with midi, and also a switch to change between wave-forms.

In this version we add Attack and Release envelopes to the synth.  We could add a full ADSR envelope, but the Arduino only has so many free inputs and I'm trying to keep this simple.  If you still want to, you could figure it out by looking at the code I'm sure, but when we add more features down the line, you won't have enough inputs unless you start using a multiplexer.  Maybe we'll end up doing that anyway, we'll see.

You'll need two potentiometers (I'm using B10k ones)

Connect center pot pin to;

A5: for Atk

A4: for Release

connect the other pot pins to ground and 5v. 



To stop mis-triggering on the attack I have two 1k resistors in parallel (amounting to 500 ohms resistance) on the ground input of the attack pot.  You could put two 200(ish)ohm resisters in series instead (it doesn't have to be exactly 500 ohms), or play with the code...  maybe set the int val of the atkVal to something over 20 instead of 0?  The resistors work for me, so I'm sticking to that.

Here's a link to the code;

https://github.com/gary909/Helios-Arduino-Synth-V2

When you upload the code, remember you need to disconnect the RX pin on the Arduino.  You also need to have the Mozzi and Midi library's installed on the Arduino software.

Hopefully part 3 won't take me quite as long this time around.

Thanks.
