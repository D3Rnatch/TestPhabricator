
#include "mpu_handler.h"

volatile bool mpuInterrupt = false;
void MPU_Handler :: dmpDataReady()
{
    // ////Serial.println("INTERRUPT !");
    mpuInterrupt = true;
}


MPU_Handler :: MPU_Handler()
{
        dmpReady = false;  // set true if DMP init was successful
        fifoCount = 0; // INIT DAMN
	// INIT OF MPU
	this->mpu = new MPU6050(0x69); // AD0 High for ATMEGA2650
        this->prec = 0.0;
        this->actual = -1.0;
        this->stabilized = false;
        this->g_value = 0;
        this->threshold = 0;
	mpu->initialize();
	
	mpu->testConnection() ? this->isConnected = true : this->isConnected = false;
	//////Serial.println("Passed connection");
	if(this->isConnected) {	
  	  //      ////Serial.println("Passed connection");

			this->devStatus = mpu->dmpInitialize();
		    // supply your own gyro offsets here, scaled for min sensitivity
			mpu->setXGyroOffset(220);
			mpu->setYGyroOffset(76);
			//mpu.setZGyroOffset(-85);
			mpu->setZAccelOffset(1788); // 1688 factory default for my test chip
			
			if (devStatus == 0) {
                        //        ////Serial.println("DMP reached.");
			// turn on the DMP, now that it's ready
			mpu->setDMPEnabled(true);

			// enable Arduino interrupt detection
			attachInterrupt(0, dmpDataReady, RISING);
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
      
        delay(2);
        // Stabilize mpu :
        
        while(!this->isStabilized()){ this->run_the_magic(); }
        
        // Serial.print("init finished. : ");
        // Serial.println(this->threshold,DEC);
}

void MPU_Handler :: run_the_magic()
{
        if(!dmpReady) return;
        
        // Didnot find a better idea....
        while( !mpuInterrupt && fifoCount < packetSize){ 
        
        // For looop optimization, feel free to hook here some functions
          
        }
        
                // check if stabilized
                this->checkStabilization();
                
                //Serial.println("Here we are here.");
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

                        if(this->stabilized)
                            ypr[0] -= this->threshold;
                        this->g_value = ypr[0];
		}
	// }//*/
}
		
bool MPU_Handler :: isDisponible()
{
	// Test for stabilization and determine the Offset...
        //////Serial.print("DmpReady is :");
        //////Serial.println(dmpReady);
	if(mpuInterrupt && fifoCount < packetSize)
		return true;
	else
		return false;
}
		
void MPU_Handler :: calibrate_IMU()
{
  // TO BE IMPLEMENTED
}

bool MPU_Handler :: isStabilized()
{
    return this->stabilized;
}

void MPU_Handler :: checkStabilization()
{
 if(!this->stabilized)
  {
   this->prec = this->actual;
   int g = this->getGValue();
   this->actual = g;
   if(prec == actual && nb <100)
          nb++;
   else if (prec != actual)
         nb = 0;
   else {
        
        //Serial.println("HERE.");
        this->stabilized = true;
         this->threshold = this->actual;        
  }  
}
//else
  //////Serial.println("Stab ok.");
  //////Serial.println(this->threshold,DEC);
}

	
int MPU_Handler :: getGValue()
{
	return this->g_value;
}

int MPU_Handler :: getErrorCode()
{
       return this->devStatus; 
}
