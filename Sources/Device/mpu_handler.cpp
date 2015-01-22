
#include "MPU_Handler.h"

volatile bool mpuInterrupt = false;
void MPU_Handler :: dmpDataReady()
{
    mpuInterrupt = true;
}


MPU_Handler :: MPU_Handler()
{
        dmpReady = false;  // set true if DMP init was successful
        
	// INIT OF MPU
	this->mpu = new MPU6050(0x69); // AD0 High for ATMEGA2650

	// join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

	mpu->initialize();
	
	mpu->testConnection() ? this->isConnected = true : this->isConnected = false;
	
	if(this->isConnected) {	
			this->devStatus = mpu->dmpInitialize();
		    // supply your own gyro offsets here, scaled for min sensitivity
			mpu->setXGyroOffset(220);
			mpu->setYGyroOffset(76);
			//mpu.setZGyroOffset(-85);
			mpu->setZAccelOffset(1788); // 1688 factory default for my test chip
			
			if (devStatus == 0) {
			// turn on the DMP, now that it's ready
			mpu->setDMPEnabled(true);

			// enable Arduino interrupt detection
			attachInterrupt(0, MPU_Handler::dmpDataReady, RISING);
			mpuIntStatus = mpu->getIntStatus();

			// set our DMP Ready flag so the main loop() function knows it's okay to use it
			dmpReady = true;

			// get expected DMP packet size for later comparison
			packetSize = mpu->dmpGetFIFOPacketSize();
			// ERROR! DevStatus
			// 1 = initial memory load failed
			// 2 = DMP configuration updates failed
			// (if it's going to break, usually the code will be 1)
		}
	}
}

void MPU_Handler :: run_the_magic()
{
	if(this->isDisponible()) {
		mpuInterrupt = false;
		mpuIntStatus = mpu->getIntStatus();

		// get current FIFO count
		fifoCount = mpu->getFIFOCount();
		
		if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                // reset so we can continue cleanly
                mpu->resetFIFO();

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
		} else if (mpuIntStatus & 0x02) {
			// wait for correct available data length, should be a VERY short wait
			while (fifoCount < packetSize) fifoCount = mpu->getFIFOCount();

			// read a packet from FIFO
			mpu->getFIFOBytes(fifoBuffer, packetSize);
        
			// track FIFO count here in case there is > 1 packet available
			// (this lets us immediately read more without waiting for an interrupt)
			fifoCount -= packetSize;
			
			// récupération des données YPR
            // display Euler angles in degrees
            mpu->dmpGetQuaternion(&q, fifoBuffer);
            mpu->dmpGetGravity(&gravity, &q);
            mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
			ypr[0] = ypr[0]*180/M_PI;
			ypr[1] = ypr[1]*180/M_PI;
			ypr[2] = ypr[2]*180/M_PI;
		}
	}
}
		
bool MPU_Handler :: isDisponible()
{
	// Test for stabilization and determine the Offset...
	if(!mpuInterrupt && fifoCount < packetSize && !dmpReady)
		return true;
	else
		return false;
}
		
void MPU_Handler :: calibrate_IMU()
{
	// TO BE IMPLEMENTED
}
		
int MPU_Handler :: getGValue()
{
	return this->g_value;
}

// Error : 12 <=> WARNING IMU not connected
// Error : 22 <=> WARNING IMU dmp not reachable
// Error : 2 <=> Both
int MPU_Handler :: getErrorCode()
{
   uint8_t id = 0;
   if(!this->isConnected)
         return 12;
   else if( devStatus == 0)
         return 22;
}

