#include "ADNS2610_arduino_driver.h"

void ADNSmouseInit(void)
{
	// pins init...
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT); 

  pinMode(DLEDY, OUTPUT);
  pinMode(DLEDR, OUTPUT);
  pinMode(DLEDG, OUTPUT);
  pinMode(DLEDPERF, OUTPUT);

  ADNSflash(DLEDY, 1);
  ADNSflash(DLEDR, 1);
  ADNSflash(DLEDG, 1);
  ADNSflash(DLEDPERF, 1);

  flop = false;

  delayMicroseconds(5);
  // MOUSE REAL INIT 
  
  digitalWrite(SCLK, HIGH);
  delayMicroseconds(5);
  digitalWrite(SCLK, LOW);
  delayMicroseconds(1);
  digitalWrite(SCLK, HIGH);
  delay(1025);
  ADNSwriteRegister(regConfig, maskNoSleep); //Force the mouse to be always on.
  
  ADNSdumpDiag();
}

void ADNSdumpDiag(void)
{
  unsigned int val;

  val = ADNSreadRegister(regStatus);

  Serial.print("Product ID: ");
  Serial.println( (unsigned int)((val & maskPID) >> 5));
  Serial.println("Ready.");
  Serial.flush();
}

void ADNSwriteRegister(byte addr, byte data)
{
  byte i;

  addr |= 0x80; //Setting MSB high indicates a write operation.

  //Write the address
  pinMode (SDIO, OUTPUT);
  for (i = 8; i != 0; i--)
  {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, addr & (1 << (i-1) ));
    digitalWrite (SCLK, HIGH);
  }

  //Write the data    
  for (i = 8; i != 0; i--)
  {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, data & (1 << (i-1) ));
    digitalWrite (SCLK, HIGH);
  }
}

byte ADNSreadRegister(byte addr)
{
  byte i;
  byte r = 0;

  //Write the address
  pinMode (SDIO, OUTPUT);
  for (i = 8; i != 0; i--)
  {
    digitalWrite (SCLK, LOW);
    digitalWrite (SDIO, addr & (1 << (i-1) ));
    digitalWrite (SCLK, HIGH);
  }

  pinMode (SDIO, INPUT);  //Switch the dataline from output to input
  delayMicroseconds(110);  //Wait (per the datasheet, the chip needs a minimum of 100 µsec to prepare the data)

  //Clock the data back in
  for (i = 8; i != 0; i--)
  {                             
    digitalWrite (SCLK, LOW);
    digitalWrite (SCLK, HIGH);
    r |= (digitalRead (SDIO) << (i-1) );
  }

  delayMicroseconds(110);  //Tailing delay guarantees >100 µsec before next transaction

  return r;
}


void ADNSreadFrame(byte *arr)
{
  byte *pos;
  byte *uBound;
  unsigned long timeout;
  byte val;

  //Ask for a frame dump
  ADNSwriteRegister(regPixelData, 0x2A);

  val = 0;
  pos = arr;
  uBound = arr + 325;

  timeout = millis() + 1000;

  //There are three terminating conditions from the following loop:
  //1. Receive the start-of-field indicator after reading in some data (Success!)
  //2. Pos overflows the upper bound of the array (Bad! Might happen if we miss the start-of-field marker for some reason.)
  //3. The loop runs for more than one second (Really bad! We're not talking to the chip properly.)
  while( millis() < timeout && pos < uBound)
  {
    val = ADNSreadRegister(regPixelData);

    //Only bother with the next bit if the pixel data is valid.
    if( !(val & 64) )
      continue;

    //If we encounter a start-of-field indicator, and the cursor isn't at the first pixel,
    //then stop. ('Cause the last pixel was the end of the frame.)
    if( ( val & 128 ) 
      &&  ( pos != arr) )
      break;

    *pos = val & 63;
    pos++;
  }

  ADNSflipLED();
}

void ADNSflash(byte pin, byte nTimes)
{
  while( nTimes-- )
  {
    digitalWrite(pin, HIGH);
    delay(120);
    digitalWrite(pin, LOW);
    delay(80);
  } 
}

void ADNSflipLED(void)
{
  flop = !flop;
  digitalWrite(DLEDY, flop ? HIGH : LOW);
}

int ADNSgetFilteredValueX()
{
	return ADNSTresholdFilter(ADNSgetTrueValue((int)ADNSreadRegister(regDeltaX)));
}

int ADNSgetFilteredValueY()
{
	return ADNSTresholdFilter(ADNSgetTrueValue((int)ADNSreadRegister(regDeltaY)));
}

int ADNSgetTrueValue(byte data)
{
	int ret = data;
	if(data > 127)
		ret -= 255;

	return ret;
}

int ADNSTresholdFilter(int data)
{
	if(data <= TRESHOLD_MAX && data >= TRESHOLD_MIN)
		return 0; 
	else
	{
		if(data < 0)
			return (data - TRESHOLD_MIN);
		else
			return (data - TRESHOLD_MAX);
	}
}

int ADNSgetSqual ()
{
   return (int)ADNSreadRegister(regSqual);  
}
