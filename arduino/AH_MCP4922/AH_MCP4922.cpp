
/*************************************************************************
**  Device: MCP4922                                                 	**
**  File:   AH_MCP4922.h - Library for get out analog voltage          	**
**								    	**
**  Created by A. Hinkel 2011-12-28  					**
**  download from "http://www.alhin.de/arduino"  			**
**					                               	**
**  Based on code from:							**
**  http://www.sinneb.net/2010/06/mcp4922-12bit-dac-voltage-controller/ **
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
**                                                                  	**
**************************************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AH_MCP4922.h"


AH_MCP4922::AH_MCP4922(int SDI, int SCK, int CS, boolean DAC, boolean GAIN)
{
 _DATAOUT = SDI;
 _SPICLOCK = SCK;
 _SLAVESELECT = CS;
 _DAC = DAC;
 _GAIN = GAIN;

 pinMode(_DATAOUT,OUTPUT);
 pinMode(_SPICLOCK,OUTPUT);
 pinMode(_SLAVESELECT,OUTPUT);

}

//************************************************************************
void AH_MCP4922::setValue(int Value) {
  sendIntValueSPI(Value);
}
//************************************************************************ 
void AH_MCP4922::sendSPIClock() {
  digitalWrite(_SPICLOCK, HIGH);
  digitalWrite(_SPICLOCK, LOW);
//  delay(1);
  delayMicroseconds(1);
}

//************************************************************************ 
void AH_MCP4922::sendSPIHeader() {
  					// bit 15
					// 0 write to DAC_A 
					// 1 write to DAC_B
  digitalWrite(_DATAOUT,_DAC);
  sendSPIClock();
					// bit 14 Vref input buffer control
					// 0 unbuffered
					// 1 buffered
  digitalWrite(_DATAOUT,LOW);
  sendSPIClock();
					// bit 13 Output Gain selection
					// 0 2x
					// 1 1x
  digitalWrite(_DATAOUT,_GAIN);
  sendSPIClock();
					// bit 12 Output shutdown control bit
					// 0 Shutdown the device
					// 1 Active mode operation *
  digitalWrite(_DATAOUT,HIGH);
  sendSPIClock();
}
 
//************************************************************************
void AH_MCP4922::sendIntValueSPI(int value) {
 // initiate data transfer with 4922
 digitalWrite(_SLAVESELECT,LOW);
 
 // send 4 bit header
 sendSPIHeader();
 
  // send data
  for(int i=11;i>=0;i--){
    digitalWrite(_DATAOUT,((value&(1<<i)))>>i);
    sendSPIClock();
  }
 
  // finish data transfer
  digitalWrite(_SLAVESELECT,HIGH);
} 
 
 

 