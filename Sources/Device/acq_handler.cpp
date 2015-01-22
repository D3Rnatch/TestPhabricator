#include "acq_handler.h"
/*
boolean dmpReady;

void dmpDataReady(){
  dmpReady = true;
}*/

ACQ_handler::ACQ_handler()
{

  
    ADNSmouseInit(); // Démarrage du capteur optique.
    // mpuInit(); // Démarrage du G sensor
    this->delta_x_adns = this->delta_y_adns = 0;
    this->g_z_mpu = 0;
    this->actual_r = this->actual_theta = last_x = last_y = 0.0;
		
    //this->mpu = new MPU6050(0x69);
    //mpu->initialize();
    //if(!mpu->testConnection())
    //this->errorCode = 112;

    //this->devStatus = mpu->dmpInitialize();

    //this->mpu->setXGyroOffset(220);
    //this->mpu->setYGyroOffset(76);
    //this->mpu->setZGyroOffset(-85);	
    /*
    if (devStatus == 0) {
	this->mpu->setDMPEnabled(true);	
        attachInterrupt(0, dmpDataReady, RISING);
	this->mpuIntStatus = mpu->getIntStatus();
	dmpReady = true;
	this->packetSize = mpu->dmpGetFIFOPacketSize();
    }*/
}

void ACQ_handler::run_the_magic()
{
    this->delta_y_adns = ADNSgetFilteredValueY();
    this->delta_x_adns = ADNSgetFilteredValueX();
    this->update_values();
}

void ACQ_handler::acquire_mpu()
{/*
    if (!dmpReady) return;
    while (!this->mpuInterrupt && this->fifoCount < this->packetSize) {
    }
    this->mpuInterrupt = false;
    this->mpuIntStatus = this->mpu->getIntStatus();

    // get current FIFO count
    this->fifoCount = this->mpu->getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    	mpu->resetFIFO();			 
    }else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) 
	    fifoCount = mpu->getFIFOCount();
        mpu->getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
	this->mpu->dmpGetQuaternion(&this->q, this->fifoBuffer);
        this->mpu->dmpGetEuler(this->euler, &this->q);
        this->mpu->dmpGetGravity(&this->gravity, &this->q);
        this->mpu->dmpGetYawPitchRoll(this->ypr, &this->q, &this->gravity);
    }*/
}

void ACQ_handler::update_values()
{
    /* Theta Angle */
	this->last_x += this->delta_x_adns;
	this->last_y += this->delta_y_adns;
	
    /*int e = (int)ALPHA << 10;
    int xe = this->delta_x_adns * e;
    int ye = this->delta_y_adns * e;
    double x = (double)(xe >> 10);
    double y = (double)(ye >> 10);
    this->last_x = x;
    this->last_y = y;		
    double t = this->actual_theta = atan2(y,x);

    /* Radius Value */
    int cT = (int)cos(t) << 10;
    int xt = xe;
    if(!cT) {
	cT = (int)sin(t) << 10;
	xt = ye;
    }
    this->actual_r = (double)((xt/cT) >> 10); */
}

void ACQ_handler::mpuCalibrate()
{
// TO BE IMPLEMENTED
}

double ACQ_handler::get_MoveXY()
{		
    return this->actual_r; // r	
}

double ACQ_handler::get_MoveAngle()
{		
    return this->actual_theta; // r	
}

int ACQ_handler::get_MoveX()
{
	int x = this->last_x;
	this->last_x = 0;
    return x;
}

int ACQ_handler::get_MoveY()
{
	int y = this->last_y;
	this->last_y = 0;
    return y;
}

///////////////////////////////////////////////////////
// ADNS ZONE

void ACQ_handler::ADNSmouseInit(void)
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

void ACQ_handler::ADNSdumpDiag(void)
{
  unsigned int val;

  val = ADNSreadRegister(regStatus);
/*
  Serial.print("Product ID: ");
  Serial.println( (unsigned int)((val & maskPID) >> 5));
  Serial.println("Ready.");
  Serial.flush();*/
}

void ACQ_handler::ADNSwriteRegister(byte addr, byte data)
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

byte ACQ_handler::ADNSreadRegister(byte addr)
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


void ACQ_handler::ADNSreadFrame(byte *arr)
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

void ACQ_handler::ADNSflash(byte pin, byte nTimes)
{
  while( nTimes-- )
  {
    digitalWrite(pin, HIGH);
    delay(120);
    digitalWrite(pin, LOW);
    delay(80);
  } 
}

void ACQ_handler::ADNSflipLED(void)
{
  flop = !flop;
  digitalWrite(DLEDY, flop ? HIGH : LOW);
}

int ACQ_handler::ADNSgetFilteredValueX()
{
	return ADNSTresholdFilter(ADNSgetTrueValue((int)ADNSreadRegister(regDeltaX)));
}

int ACQ_handler::ADNSgetFilteredValueY()
{
	return ADNSTresholdFilter(ADNSgetTrueValue((int)ADNSreadRegister(regDeltaY)));
}

int ACQ_handler::ADNSgetTrueValue(byte data)
{
	int ret = data;
	if(data > 127)
		ret -= 255;

	return ret;
}

int ACQ_handler::ADNSTresholdFilter(int data)
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

int ACQ_handler::ADNSgetSqual ()
{
   return (int)ADNSreadRegister(regSqual);  
}


