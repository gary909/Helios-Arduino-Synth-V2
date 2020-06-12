//
//
//
//
// /$$   /$$ /$$$$$$$$ /$$       /$$$$$$  /$$$$$$   /$$$$$$         /$$$$$$  /$$   /$$ /$$$$$$$$          
//| $$  | $$| $$_____/| $$      |_  $$_/ /$$__  $$ /$$__  $$       /$$__  $$| $$$ | $$| $$_____/          
//| $$  | $$| $$      | $$        | $$  | $$  \ $$| $$  \__/      | $$  \ $$| $$$$| $$| $$                
//| $$$$$$$$| $$$$$   | $$        | $$  | $$  | $$|  $$$$$$       | $$  | $$| $$ $$ $$| $$$$$             
//| $$__  $$| $$__/   | $$        | $$  | $$  | $$ \____  $$      | $$  | $$| $$  $$$$| $$__/             
//| $$  | $$| $$      | $$        | $$  | $$  | $$ /$$  \ $$      | $$  | $$| $$\  $$$| $$                
//| $$  | $$| $$$$$$$$| $$$$$$$$ /$$$$$$|  $$$$$$/|  $$$$$$/      |  $$$$$$/| $$ \  $$| $$$$$$$$          
//|__/  |__/|________/|________/|______/ \______/  \______/        \______/ |__/  \__/|________/          
//                                                                                                        
//                                                                                                        
//                                                                                                        
//                                 /$$     /$$                           /$$                              
//                                | $$    | $$                          |__/                              
//  /$$$$$$$ /$$   /$$ /$$$$$$$  /$$$$$$  | $$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$$$  /$$$$$$   /$$$$$$ 
// /$$_____/| $$  | $$| $$__  $$|_  $$_/  | $$__  $$ /$$__  $$ /$$_____/| $$|____ /$$/ /$$__  $$ /$$__  $$
//|  $$$$$$ | $$  | $$| $$  \ $$  | $$    | $$  \ $$| $$$$$$$$|  $$$$$$ | $$   /$$$$/ | $$$$$$$$| $$  \__/
// \____  $$| $$  | $$| $$  | $$  | $$ /$$| $$  | $$| $$_____/ \____  $$| $$  /$$__/  | $$_____/| $$      
// /$$$$$$$/|  $$$$$$$| $$  | $$  |  $$$$/| $$  | $$|  $$$$$$$ /$$$$$$$/| $$ /$$$$$$$$|  $$$$$$$| $$      
//|_______/  \____  $$|__/  |__/   \___/  |__/  |__/ \_______/|_______/ |__/|________/ \_______/|__/      
//           /$$  | $$                                                                                    
//          |  $$$$$$/                                                                                    
//           \______/     
//
//                                                                 /$$$$$$ 
//                                                                /$$__  $$
//                                                     /$$    /$$|__/  \ $$
//                                                    |  $$  /$$/  /$$$$$$/
//                                                     \  $$/$$/  /$$____/ 
//                                                      \  $$$/  | $$      
//                                                       \  $/   | $$$$$$$$
//                                                        \_/    |________/
//
// A BlogHoskins Monstrosity @ 2019
// https://bloghoskins.blogspot.com/

/*  
 *   v2
 *   This version adds Attack & Release on the analog inputs. You'll need 2 pots. 
 *   Connect center pot pin to;
 *   A5: for Atk
 *   A4: for Release
 *   connect the other pot pins to ground and 5v. 
 *   To stop mis-triggering on the atk I have x2 1k resistors in parallel (amounting 
 *   to 500 ohms resistance) on the ground input of the atk pot. you could put two 
 *   200(ish)ohm resisters in series instead, or play with the code...  maybe set the
 *   int val of the atkVal to something over 20 instead of 0?
 *   
 *   
 *   v1
 *   http://bloghoskins.blogspot.com/2019/07/helios-one-arduino-synth-part-1.html
 *   This vesrion of code lets you set between SQR and SAW wave with a 
 *   switch (input 2 on arduino)
 *   MIDI is working.
 *   You'll need to install the MIDI & Mozzi arduino libraries to get it to work.
*/


#include <MIDI.h>

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/saw2048_int8.h> // saw table for oscillator
#include <tables/square_no_alias_2048_int8.h> // square table for oscillator
#include <mozzi_midi.h>
#include <ADSR.h>


MIDI_CREATE_DEFAULT_INSTANCE();


#define WAVE_SWITCH 2 // switch for switching waveforms


// Set up Attack Envelope
const int atkPot = A5;    // select the input pin for the potentiometer
int atkVal = 0;       // variable to store the value coming from the pot
const int dkyPot = A4;    // select the input pin for the potentiometer
int dkyVal = 0;       // variable to store the value coming from the pot

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128 // powers of 2 please

// audio sinewave oscillator
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> oscil1; //Saw Wav
Oscil <SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscil2; //Sqr wave

// envelope generator
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;

#define LED 13 // Internal LED lights up if MIDI is being received

void HandleNoteOn(byte channel, byte note, byte velocity) { 
  oscil1.setFreq(mtof(float(note)));
  envelope.noteOn();
  digitalWrite(LED,HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) { 
  envelope.noteOff();
  digitalWrite(LED,LOW);
}

void setup() {
//  Serial.begin(9600); // see the output
  pinMode(LED, OUTPUT);  // built-in arduino led lights up with midi sent data 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function
  // Initiate MIDI communications, listen to all channels (not needed with Teensy usbMIDI)
  MIDI.begin(MIDI_CHANNEL_OMNI);  
  envelope.setADLevels(255,64); // A bit of attack / decay while testing
  oscil1.setFreq(440); // default frequency
  startMozzi(CONTROL_RATE); 
}


void updateControl(){
  MIDI.read();
  envelope.update();
  atkVal = mozziAnalogRead(atkPot);    // read the value from the attack pot
  dkyVal = mozziAnalogRead(dkyPot);    // read the value from the decay pot
  envelope.setTimes(atkVal,5000,10000,dkyVal); // 10000 is so the note will sustain 10 seconds unless a noteOff comes
  pinMode(2, INPUT_PULLUP); // Pin two is connected to the middle of a switch, high switch goes to 5v, low to ground
  int sensorVal = digitalRead(2); // read the switch position value into a  variable
  if (sensorVal == HIGH) // If switch is set to high, run this portion of code
  {
    oscil1.setTable(SAW2048_DATA);
  }
  else  // If switch not set to high, run this portion of code instead
  {
    oscil1.setTable(SQUARE_NO_ALIAS_2048_DATA);
  }
}


int updateAudio(){
  return (int) (envelope.next() * oscil1.next())>>8;
}


void loop() {
  audioHook(); // required here
} 