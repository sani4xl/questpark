// this library required https://github.com/reeedstudio/libraries/tree/master/DigitalTube
#include "TM1637.h"
#define CLK 8//pins definitions for TM1637 and can be changed to other ports       
#define DIO 9
//TM1637Display tm1637(CLK,DIO);
TM1637 tm1637(CLK,DIO);

String suka = "4867";
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
char buffer[4];


void setup()
{
  Serial.begin(9600);
 suka.toCharArray(buffer, 5);
  //Serial.println(buffer[0]);
  for(int i = 0; i<4; i++){
    TimeDisp[i]= buffer[i] - 48;
  }
  tm1637.set();//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.init();
}

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
void loop()
{
  //TimeUpdate("6789");
  tm1637.display(TimeDisp);
  /*tm1637.setColon(true);
  for(int k = 0; k < 4; k++)
	data[k] = k;
  tm1637.setSegments(data);
  */
  
  tm1637.point(true);
  
}


void TimeUpdate(String stringVar){

  unsigned char charVar[sizeof(stringVar)]; 
  //stringVar.toCharArray(charVar, sizeof(charVar));
  //unsigned char newData = charVar; 
  
 
  TimeDisp[0] = charVar[0];
  TimeDisp[1] = charVar[1];
  TimeDisp[2] = charVar[2];
  TimeDisp[3] = charVar[3];
}
