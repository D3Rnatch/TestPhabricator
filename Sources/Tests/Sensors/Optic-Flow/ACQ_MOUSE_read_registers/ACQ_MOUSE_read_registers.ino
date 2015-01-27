#include <Arduino.h>
#include "ADNS2610_arduino_driver.h"

void setup()
{
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT); 

  pinMode(DLEDY, OUTPUT);
  pinMode(DLEDR, OUTPUT);
  pinMode(DLEDG, OUTPUT);
  pinMode(DLEDPERF, OUTPUT);

  flash(DLEDY, 1);
  flash(DLEDR, 1);
  flash(DLEDG, 1);
  flash(DLEDPERF, 1);

  flop = false;

  Serial.begin(38400);
  Serial.println("Serial established.");
  Serial.flush();

  mouseInit();
  dumpDiag();  
}


void loop()
{
 // int input;
 double dX, dY;
      dX = (double)readRegister(regDeltaX);
      dY = (double)readRegister(regDeltaY);
      Serial.print(dX, DEC);
      Serial.print(":");
      Serial.println(dY, DEC);
      Serial.println("");/*
      Serial.print("Addr : 0x01");
      Serial.println(readRegister(0x01),BIN);
      Serial.print("Addr : 0x04");
      Serial.println(readRegister(0x04),BIN);
      Serial.print("Addr : 0x05");
      Serial.println(readRegister(0x05),BIN);
      Serial.print("Addr : 0x06");
      Serial.println(readRegister(0x06),BIN);
      Serial.print("Addr : 0x07");
      Serial.println(readRegister(0x07),BIN);
      Serial.print("Addr : 0x08");
      Serial.println(readRegister(0x08),BIN);
      Serial.print("Addr : 0x09");
      Serial.println(readRegister(0x09),BIN);
      Serial.print("Addr : 0x0A");
      Serial.println(readRegister(0x0A),BIN);*/
     delay(100);
}
