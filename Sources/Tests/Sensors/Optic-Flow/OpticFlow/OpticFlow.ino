#include <Arduino.h>
#include "ADNS2610_arduino_driver.h"

void setup() 
{
  Serial.begin(38400);
  Serial.println("Serial is initiated.");
  Serial.flush();
  mouseInit();
}

void loop()
{
  unsigned int s;
  int input;  

  readFrame(frame);

  if( Serial.available() )
  {
    input = Serial.read();
    switch( input )
    {
    case 'f':
      Serial.println("Frame capture.");
      readFrame(frame);
      Serial.println("Done.");
      break;
    case 'd':
      for( input = 0; input < FRAMELENGTH; input++ )  //Reusing 'input' here
        Serial.print( (byte) frame[input] );
      Serial.print( (byte)127 );
      break;
    }
    Serial.flush();
  }
}
