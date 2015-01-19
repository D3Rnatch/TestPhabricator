#ifndef ADNS2610_DRIVER_H
#define ADNS2610_DRIVER_H

/**
 * \file ADNS2610_arduino_driver.h
 * \brief ADNS2610 driver for Arduino.
 * \author Alexandre Brand
 * \version 1.0
 */

#include <Arduino.h>

#define SDIO 50 /*!< SPI protocol wire SDIO Arduino pin. */
#define SCLK 52 /*!< SPI protocol wire SCLK Arduino pin. */
//#define SDIO 3
//#define SCLK 2

#define TRESHOLD_MIN -20 /*!< Min treshold. */
#define TRESHOLD_MAX 20 /*!< Max treshold. */

#define DLEDG 9
#define DLEDR 10
#define DLEDY 11
#define DLEDPERF 13

#define FRAMELENGTH 324
byte frame[FRAMELENGTH];

byte flop;

const byte regConfig    = 0x00;
const byte regStatus    = 0x01;
const byte regPixelData = 0x08;
const byte maskNoSleep  = 0x01;
const byte maskPID      = 0xE0;
const byte regDeltaX    = 0x02;
const byte regDeltaY    = 0x03;
const byte regSqual     = 0x04;

/**
 * \fn void ADNSmouseInit(void)
 */
void ADNSmouseInit(void);
/**
 * \fn void ADNSdumpDiag(void)
 */
void ADNSdumpDiag(void);
/**
 * \fn void ADNSwriteRegister(byte addr, byte data)
 */
void ADNSwriteRegister(byte addr, byte data);
/**
 * \fn void ADNSreadRegister(byte addr)
 */
byte ADNSreadRegister(byte addr);
/**
 * \fn void ADNSreadFrame(byte *arr)
 * \brief ADNS2610 dumps a 324-byte array, so this function assumes arr points to a buffer of at least 324 bytes.
 */
void ADNSreadFrame(byte *arr);
/**
 * \fn void ADNSflash(byte pin, byte nTimes)
 */
void ADNSflash(byte pin, byte nTimes);
/**
 * \fn void ADNSflipLED(void)
 */
void ADNSflipLED(void);

// HIGH LEVEL FUNCTIONS
/**
 * \fn void ADNSgetTrueValue(byte data)
 * \brief Allows negative values.
 */
int ADNSgetTrueValue(byte data);
/**
 * \fn void ADNSTresholdFilter(int data)
 */
int ADNSTresholdFilter(int data);
/**
 * \fn void ADNSgetFilteredValueY()
 */
int ADNSgetFilteredValueY();
/**
 * \fn void ADNSgetFilteredValueX()
 * \brief Returns DeltaX value using the ThresholdFilter.
 */
int ADNSgetFilteredValueX();
/**
 * \fn void ADNSgetSqual()
 */
int ADNSgetSqual();


#endif
