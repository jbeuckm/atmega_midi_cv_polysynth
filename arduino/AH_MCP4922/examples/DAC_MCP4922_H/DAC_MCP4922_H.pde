/*************************************************************************
**  AH_MCP4922.h - Library for get out analog voltage          	        **
**  Created by A. Hinkel 2012-01-05					**
**  download from "http://www.alhin.de/arduino"  			**
**									**
**  Based on code from website: 					**
**  http://www.sinneb.net/2010/06/mcp4921-12bit-dac-voltage-controller/ **
**									**
**  Released into the public domain.  		                    	**
*************************************************************************/

#include "AH_MCP4922.h"

#define AI1 A1                                  //Analog input monitor 1
#define AI2 A2                                  //Analog input monitor 2

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 
AH_MCP4922 AnalogOutput1(51,52,53,LOW,HIGH);    //define AnalogOutput1 for MEGA_board, select DAC_A, Gain=1x
AH_MCP4922 AnalogOutput2(51,52,53,HIGH,LOW);    //define AnalogOutput2 for MEGA_board, select DAC_B, Gain=2x

/********************************
*            | MEGA  |  UNO     *
*  --------------------------   *
*  SCK_pin   |  52   |   13     *
*  MOSI_pin  |  51   |   11     *
*  CS_pin    |  53   |   12     *
*********************************
* DAC=LOW  => DAC_A select	*
* DAC=HIGH => DAC_B select	*
*				*
* GAIN=LOW  => 2x gain select	*
* GAIN=HIGH => 1x gain select	*
********************************/

//***************************************************************** 
void setup() { 
  AnalogOutput1.setValue(4000);
  AnalogOutput2.setValue(0);
  delay(1000);
  pinMode(AI1,INPUT);                            //Voltage measurement pin1
  pinMode(AI2,INPUT);                            //Voltage measurement pin2
   
  Serial.begin(9600);                            //Init serial interface 
  Serial.println("Ready");        
} 

//*****************************************************************
void loop() {
 for (int i=0; i<4096; i+=8)
 {
  Serial.print(i,DEC);
  AnalogOutput1.setValue(i);                     //set voltage for AnalogOutput1 
  AnalogOutput2.setValue(i);                     //set volateg for AnalogOutput2
  
  Serial.print(";");
  delay(10);

  int AO1 = analogRead(AI1);                     //check the input voltage AI1 (pin A1)
  int AO2 = analogRead(AI2);                     //check the input voltage AI2 (pin A2)
  delay(10);
  
  Serial.print("AO1="); 
  Serial.print(AO1);
  Serial.print(" ; AO2=");  
  Serial.println(AO2);
 }
}
 

