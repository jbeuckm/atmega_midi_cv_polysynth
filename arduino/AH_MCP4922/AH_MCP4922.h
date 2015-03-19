
/*************************************************************************
**  Device: MCP4922                                                 	**
**  File:   AH_MCP4922.h - Library for get out analog voltage          	**
**								    	**
**  Created by A. Hinkel 2011-12-28                                 	**
**  download from "http://www.alhin.de/arduino"  			**
**									**
**  Based on Code from:							**
**  http://www.sinneb.net/2010/06/mcp4921-12bit-dac-voltage-controller/ **
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
**                                                                  	**
*************************************************************************/


#ifndef AH_MCP4922_h
#define AH_MCP4922_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class AH_MCP4922
{
  public:
    AH_MCP4922(int SDI, int SCK,int CS, boolean DAC, boolean GAIN);
    void setValue(int Value);

  private:
    int _DATAOUT;
    int _SPICLOCK;
    int _SLAVESELECT;
    boolean _DAC;
    boolean _GAIN;

    void sendSPIHeader();
    void sendIntValueSPI(int value);
    void sendSPIClock();
};

#endif

/******************************** 
*				*
* DAC=LOW  => DAC_A select	*
* DAC=HIGH => DAC_B select	*
*				*
* GAIN=LOW  => 2x gain select	*
* GAIN=HIGH => 1x gain select	*
*				*
********************************/