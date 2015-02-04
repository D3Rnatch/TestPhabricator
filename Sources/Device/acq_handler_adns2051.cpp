

#include "acq_handler_adns2051.h"

ACQ_handler::ACQ_handler()
{

  
    // ADNSmouseInit(); // Démarrage du capteur optique.
    // mpuInit(); // Démarrage du G sensor
    // Starting mouse object
    optical = new ADNS2051(SCLK,SDIO);
    optical->begin();                       // Resync (not really necessary?)
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
    this->optical->updateStatus();
    //Serial.print("Updated Status");
    //this->delta_y_adns = ADNSgetFilteredValueY();
    //this->delta_x_adns = ADNSgetFilteredValueX();
    this->delta_y_adns = this->optical->dx();
    this->delta_x_adns = this->optical->dy();
    //Serial.println(this->delta_y_adns,DEC);
    this->update_values();
}

void ACQ_handler::update_values()
{
    /* Theta Angle */
	this->last_x += this->delta_x_adns;
	this->last_y += this->delta_y_adns;

        /*this->last_x *= 0,0172;
        this->last_y *= 0,0172;*/

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
  /*
    float f_x = this->last_x * MAGIC_TO_CM;


        this->last_x = 0;


    this->real_x = f_x;

    return (int)this->real_x; */

  int x = this->last_x;
  this->last_x = 0;
  return x;
}

int ACQ_handler::get_MoveY()
{
  /*
        float f_y = this->last_y * MAGIC_TO_CM;

        this->last_y = 0;
                
        this->real_y = f_y;
        Serial.println(real_y,DEC);
        return (int)this->real_y;*/
        
        
  int x = this->last_y;
  this->last_y = 0;
  return x;
}
void ACQ_handler :: position_calculation()
{
	/*
        float f_x = this->last_x * MAGIC_TO_CM;
        float f_y = this->last_y * MAGIC_TO_CM;
        
        // reset of last_x et last_y
        this->last_x = 0;
        this->last_y = 0;
        
        this->real_x = f_x;
        this->real_y = f_y;
        
        //Serial.print("DIST;");
        Serial.print(this->f_x,DEC);
        Serial.print(";");
        Serial.println(this->f_y,DEC);*/
}
				
void ACQ_handler :: speed_calculation(){
			// to be implemented
}

///////////////////////////////////////////////////////
// ADNS ZONE
int ACQ_handler :: ADNSgetFilteredValueY()
{
	int Y = optical->dy();
        //Serial.print("TEST is Y:");
        //Serial.println(Y,DEC);
	if(Y <= TRESHOLD_MAX || Y >= TRESHOLD_MIN)
		return 0; 
	else
	{
		if(Y < 0)
			return (Y - TRESHOLD_MIN);
		else
			return (Y - TRESHOLD_MAX);
	}
}

int ACQ_handler :: ADNSgetFilteredValueX()
{
	int X = optical->dx();
        //Serial.print("TEST is X:");
        //Serial.println(X,DEC);
        
        if(X <= TRESHOLD_MAX || X >= TRESHOLD_MIN)
		return 0; 
	else
	{
		if(X < 0)
			return (X - TRESHOLD_MIN);
		else
			return (X - TRESHOLD_MAX);
	}
}

