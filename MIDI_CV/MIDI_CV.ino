#include <MIDI.h>
#include "AH_MCP4922.h"

#define LED 13   		    // LED pin on Arduino Uno
#define Gate1 1
#define Gate2 2
#define Gate3 3
#define Gate4 4


//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 

AH_MCP4922 AnalogOutput1(11,10,12,LOW,LOW);
AH_MCP4922 AnalogOutput2(11,10,12,HIGH,LOW);

AH_MCP4922 AnalogOutput3(8,7,9,LOW,LOW);
AH_MCP4922 AnalogOutput4(8,7,9,HIGH,LOW);

AH_MCP4922 Pitch1(A1,A0,A2,LOW,LOW);
AH_MCP4922 Velocity1(A1,A0,A2,HIGH,LOW);
AH_MCP4922 Pitch2(A4,A3,A5,LOW,LOW);
AH_MCP4922 Velocity3(A4,A3,A5,HIGH,LOW);


int nextNoteOutput = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

byte selectedChannel = 17;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (selectedChannel == 17) {
    selectedChannel = channel;
  }
  else if (channel != selectedChannel) {
    return;
  }
  
  liveNoteCount++;
  
  AnalogOutput1.setValue((pitch - 12) * 42);
  AnalogOutput3.setValue(velocity * 32);

  digitalWrite(LED,HIGH);
/*  
    digitalWrite(Gate1,HIGH);
    digitalWrite(Gate2,HIGH);
    */
    digitalWrite(Gate3,HIGH);
    digitalWrite(Gate4,HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  if (channel != selectedChannel) {
    return;
  }
  liveNoteCount--;
  
  if (liveNoteCount == 0) {
    digitalWrite(LED,LOW);
/*    
    digitalWrite(Gate1,LOW);
    digitalWrite(Gate2,LOW);
    */
    digitalWrite(Gate3,LOW);
    digitalWrite(Gate4,LOW);
  }
}

void handleControlChange(byte channel, byte number, byte value)
{
  if (channel != selectedChannel) {
    return;
  }
  if (number == 1) {
    AnalogOutput4.setValue(value * 32);
  }
}

void handlePitchBend(byte channel, int bend)
{
  if (channel != selectedChannel) {
    return;
  }
    AnalogOutput2.setValue((float)bend/4.0 + 2048.0);
}


// -----------------------------------------------------------------------------

void setup()
{
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);

    pinMode(LED, OUTPUT);
/*    
    pinMode(Gate1, OUTPUT);
    pinMode(Gate2, OUTPUT);
*/    
    pinMode(Gate3, OUTPUT);
    pinMode(Gate4, OUTPUT);
    
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);
    
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandlePitchBend(handlePitchBend);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    
    // C8 at full velocity for 8.0V calibration on powerup
    handleNoteOn(17, 108, 127);

}

void loop()
{
    MIDI.read();
}

