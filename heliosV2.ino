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
 *   This vesrion of code lets you set between SQR and SAW wave with a switch
 *   MIDI is working. 
 *   You'll need to install the MIDI & Mozzi libraries to get it to work
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

// NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW
// Set up Attack Envelope
int atkPot = A5;    // select the input pin for the potentiometer
int atkVal = 0;       // variable to store the value coming from the pot
int dkyPot = A4;    // select the input pin for the potentiometer
int dkyVal = 0;       // variable to store the value coming from the pot

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128 // powers of 2 please

// audio sinewave oscillator
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> oscil1;
Oscil <SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscil2;

// envelope generator
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;

#define LED 13 // Internal LED lights up if MIDI is being recieved

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
  pinMode(LED, OUTPUT);  

  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function
  // Initiate MIDI communications, listen to all channels (not needed with Teensy usbMIDI)
  MIDI.begin(MIDI_CHANNEL_OMNI);  
    
  envelope.setADLevels(255,64); // A bit of attack / decay while testing
  envelope.setTimes(200,200,10000,200); // 10000 is so the note will sustain 10 seconds unless a noteOff comes

  oscil1.setFreq(440); // default frequency
  startMozzi(CONTROL_RATE); 
}


void updateControl(){
  MIDI.read();
  envelope.update();

  // NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW
  atkVal = mozziAnalogRead(atkPot);    // read the value from the attack pot
  dkyVal = mozziAnalogRead(dkyPot);    // read the value from the decay pot
  envelope.setADLevels(atkVal,dkyVal); // A bit of attack / decay while testing
  envelope.setTimes(atkVal,200,10000,dkyVal); // 10000 is so the note will sustain 10 seconds unless a noteOff comes
//  Serial.print(dkyVal);
//  Serial.print('\n');

  // Pin two is connected to the middle of a switch, high switch goes to 5v, low to ground
  pinMode(2, INPUT_PULLUP); 
  int sensorVal = digitalRead(2); // read the switch position value into a  variable
  if (sensorVal == HIGH) // If switch is set to high, run this portion of code
  {
    oscil1.setTable(SAW2048_DATA);
  }
  else  // If switch not set to high, run thisportion of code instead
  {
    oscil1.setTable(SQUARE_NO_ALIAS_2048_DATA);
  }
}


int updateAudio(){
  return (int) (envelope.next() * oscil1.next())>>8;
}


void loop() {
//  Serial.print('\n');
//  Serial.print(atkVal);
  audioHook(); // required here
} 