#include "acq_handler.h"
boolean dmpReady;

void dmpDataReady(){
  dmpReady = true;
}

ACQ_handler :: ACQ_handler()
{
    ADNSmouseInit(); // Démarrage du capteur optique.
    // mpuInit(); // Démarrage du G sensor
    this->delta_x_adns = this->delta_y_adns = 0;
    this->g_z_mpu = 0;
    this->actual_r = this->actual_theta = last_x = last_y = 0.0;
		
    this->mpu = new MPU6050(0x69);
    mpu->initialize();
    if(!mpu->testConnection())
    this->errorCode = 112;

    this->devStatus = mpu->dmpInitialize();

    this->mpu->setXGyroOffset(220);
    this->mpu->setYGyroOffset(76);
    this->mpu->setZGyroOffset(-85);	

    if (devStatus == 0) {
	this->mpu->setDMPEnabled(true);	
        attachInterrupt(0, dmpDataReady, RISING);
	this->mpuIntStatus = mpu->getIntStatus();
	dmpReady = true;
	this->packetSize = mpu->dmpGetFIFOPacketSize();
    }
}

void ACQ_handler :: run_the_magic()
{
    this->delta_y_adns = ADNSgetFilteredValueY();
    this->delta_x_adns = ADNSgetFilteredValueX();
    this->update_values();
}

void ACQ_handler :: acquire_mpu()
{
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
    }
}

void ACQ_handler :: update_values()
{
    /* Theta Angle */
    int e = (int)ALPHA << 10;
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
    this->actual_r = (double)((xt/cT) >> 10);
}

void ACQ_handler :: mpuCalibrate()
{
// TO BE IMPLEMENTED
}

double ACQ_handler :: get_MoveXY()
{		
    return this->actual_r; // r	
}

double ACQ_handler :: get_MoveAngle()
{		
    return this->actual_theta; // r	
}

double ACQ_handler :: get_MoveX()
{
    return this->last_x;
}

double ACQ_handler :: get_MoveY()
{
    return this->last_y;
}

