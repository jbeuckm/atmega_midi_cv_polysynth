#include <MIDI.h>
#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 

AH_MCP4922 Pitch1(A1,A0,A2,LOW,LOW);
AH_MCP4922 Velocity1(A1,A0,A2,HIGH,LOW);

AH_MCP4922 Pitch2(A4,A3,A5,LOW,LOW);
AH_MCP4922 Velocity2(A4,A3,A5,HIGH,LOW);

AH_MCP4922 Pitch3(8,7,9,LOW,LOW);
AH_MCP4922 Velocity3(8,7,9,HIGH,LOW);

AH_MCP4922 Pitch4(11,10,12,LOW,LOW);
AH_MCP4922 Velocity4(11,10,12,HIGH,LOW);

AH_MCP4922 pitchDACs[4] = { Pitch1, Pitch2, Pitch3, Pitch4 };
AH_MCP4922 velocityDACs[4] = { Velocity1, Velocity2, Velocity3, Velocity4 };

int noteNumbers[4];  // keep track of notes that have been gated on
int gatePins[4] = { 2, 3, 4, 5 };

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
  
  pitchDACs[nextNoteOutput].setValue((pitch - 12) * 42);
  velocityDACs[nextNoteOutput].setValue(velocity * 32);
  digitalWrite(gatePins[nextNoteOutput], HIGH);
  
  noteNumbers[nextNoteOutput] = pitch;
  nextNoteOutput = (nextNoteOutput + 1) % 4;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  if (channel != selectedChannel) {
    return;
  }
  
  int i = 0;
  while (i < 4) {
    if (noteNumbers[i] == pitch) {
      digitalWrite(gatePins[i], LOW);
    }
    i++;
  }
}

void handleControlChange(byte channel, byte number, byte value)
{
  if (channel != selectedChannel) {
    return;
  }
  if (number == 1) {
//    AnalogOutput4.setValue(value * 32);
  }
}

void handlePitchBend(byte channel, int bend)
{
  if (channel != selectedChannel) {
    return;
  }
//    AnalogOutput2.setValue((float)bend/4.0 + 2048.0);
}


// -----------------------------------------------------------------------------

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(gatePins[0], OUTPUT);
  pinMode(gatePins[1], OUTPUT);
  pinMode(gatePins[2], OUTPUT);
  pinMode(gatePins[3], OUTPUT);
  
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
  handleNoteOn(17, 108, 127);
  handleNoteOn(17, 108, 127);
  handleNoteOn(17, 108, 127);

}

void loop()
{
    MIDI.read();
}

