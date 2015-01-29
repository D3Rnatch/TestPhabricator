

#include "acq_handler_adns2051.h"

ACQ_handler::ACQ_handler()
{

  
    // ADNSmouseInit(); // Démarrage du capteur optique.
    // mpuInit(); // Démarrage du G sensor
    // Starting mouse object
    optical.begin();                       // Resync (not really necessary?)
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
    // update sensor readables :
    this->optical.updateStatus();
    this->delta_y_adns = ADNSgetFilteredValueY();
    this->delta_x_adns = ADNSgetFilteredValueX();
    this->update_values();
}

void ACQ_handler::update_values()
{
    /* Theta Angle */
	this->last_x += this->delta_x_adns;
	this->last_y += this->delta_y_adns;

    /* Distance calculation */
    // Source is : 
    	//Calculate x,y,omega
/*
	xval = deltaX_X1_coef*acc_x[0] + deltaX_X2_coef*acc_x[1] + deltaX_Y1_minus_Y2_coef*(acc_y[0]-acc_y[1]);
	xval2 += xval;	//this cured a problem with doing xval += ...
	yval += deltaY_X1_coef*acc_x[0] + deltaY_X2_coef*acc_x[1] + deltaY_Y1_coef*acc_y[0] + deltaY_Y2_coef*acc_y[1];
	omega += omega_X1_minus_X2_coef*(acc_x[0]-acc_x[1]) + omega_Y1_minus_Y2_coef*(acc_y[0]-acc_y[1]);
		
	//Reset the accumulated x and y values for both sensors
	acc_x[0] = 0;
	acc_x[1] = 0;	
	acc_y[0] = 0;
	acc_y[1] = 0;	*/
	
    /*int e = (int)ALPHA << 10;
    int xe = this->delta_x_adns * e;
    int ye = this->delta_y_adns * e;
    double x = (double)(xe >> 10);
    double y = (double)(ye >> 10);
    this->last_x = x;
    this->last_y = y;		
    double t = this->actual_theta = atan2(y,x);

    /* Radius Value *//*
    int cT = (int)cos(t) << 10;
    int xt = xe;
    if(!cT) {
	cT = (int)sin(t) << 10;
	xt = ye;
    }
    this->actual_r = (double)((xt/cT) >> 10); */
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
int ACQhandler :: ADNSgetFilteredValueY()
{
	int Y = optical.dx();
	if (Y <= TRESHOLD_MAX) Y = 0;
	else if (Y >= TRESHOLD_MIN) Y = 0;
	else 
		Y = Y - TRESHOLD_MAX;
	return Y;

}

int ACQhandler :: ADNSgetFilteredValueX()
{
	int X = optical.dx();
	if (X <= TRESHOLD_MAX) X = 0;
	else if (X >= TRESHOLD_MIN) X = 0;
	else 
		X = X - TRESHOLD_MAX;
	return X;
}

