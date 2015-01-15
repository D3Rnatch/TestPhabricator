
#include <Arduino.h>

#include "ADNS3070_arduino_driver.h"


void setup()
{

Serial.begin(9600);

Serial.println("Serial init : [OK]");

mouseInit();

Serial.print("Revision ID is : ");
Serial.println(readRegister(0x01));
Serial.print("Motion is : ");
Serial.println(readRegister(0x02));
Serial.print("Delta_X is : ");
Serial.println(readRegister(0x03));
Serial.print("Delta_Y is : ");
Serial.println(readRegister(0x04));
Serial.print("SQUAL is : ");
Serial.println(readRegister(0x05));

Serial.println("Seemed to be working Fine ! ;)");
}

void loop()
{



}
