#include "controller_helper.h"


Controller :: Controller()
{
  	 motor_last_set = false;
	 motor_off_cpt = 0;
	 scaner_last_set = false;
	 scaner_off_cpt = 0;
  
	this->net = new Network_manager(9600);

	this->escenter[0].attach(2);
    	this->escenter[1].attach(4);
    	this->escenter[2].attach(6);
    	this->escenter[3].attach(8);
	this->LaserScaner.attach(3);
        // Serial.println("OK esc.");
	// Starting The Acq manager
	this->acq = new ACQ_handler();
	this->acq_system = 0;
        // Serial.println("End OF INIT");
        this->imu = new MPU_Handler();
}

void Controller :: init()
{


	this->calibrate_laserscaner();
    	
        this->arm();

	this->controllerState = Idle;

	// Controller finished init 
	// Sending OK ready packet.

        // Serial.print("TEST");
        uint8_t errorCode = 0;
        errorCode = this->imu->getErrorCode();
	this->net->send_ready_packet(errorCode);

}

//////////////////////////////////
//				//
//	DEFINITIONS		//
//				//
//////////////////////////////////

void Controller ::  setSpeedcent(int a, int spd){
  //On envoie la vitesse désirée
  int vit = map(spd, 0, 100, 0, 180);
  this->escenter[a].write(vit);
}

void Controller ::  setSpeedFans(uint8_t * b)
{
  int vit[4]; 
 for(int i=0;i<4;i++) {
     if(b[i]>MAX) b[i] = MAX;
     else if (b[i] < MIN) b[i] = MIN;
     vit[i] = map(b[i],0,100,0,180);
  }
 for(int j=0;j<4;j++)
     this->escenter[j].write(vit[j]);  
}

void Controller ::  setAngleLaserScaner(uint8_t angle)
{
	// int vit = map(angle,0,180,1,179);
	this->LaserScaner.write(angle);
}

void Controller ::  calibrate_laserscaner()
{
	for(int i=0;i<180;i++){
            setAngleLaserScaner(i);
            delay(10);
        }
        for(int i=180;i>0;i--){
            setAngleLaserScaner(i);
            delay(10);
        }
}

void Controller ::  arm(){
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  delay(1200);
  setSpeedcent(0,MAX);
  setSpeedcent(1,MAX);
  setSpeedcent(2,MAX);
  setSpeedcent(3,MAX);
  delay(1200);
  setSpeedcent(0,MIN);
  setSpeedcent(1,MIN);
  setSpeedcent(2,MIN);
  setSpeedcent(3,MIN);
  delay(1200);
}

void Controller ::  Process_Motor(uint8_t * b)
{
	if (this->motor_last_set == false) this->motor_last_set = true;
		       
        // we set fans power.
        setSpeedFans(b);
                
        for(int i=0;i<4;i++) {
               b[i] = 0;
        }
}

void Controller ::  Process_Scan(uint8_t * b)
{
	if (this->scaner_last_set == false){
		 // Setting to point 0 the scaner
		 // setAngleLaserScaner(0);		
		 this->scaner_last_set = true;
	}
	setAngleLaserScaner (b[0]);
	for(int i=0;i<4;i++) {
		b[i] = 0;
	}

}

void Controller ::  reset_Services()
{
	// TO BE IMPLEMENTED

	switch(this->controllerState)
	{
		case Idle :
			break;

		case Scan :
			break;

		case Manual :
			break;

		case Manual_Acquisition :
			break;
		
		default :
			break;
	}
}

void Controller ::  Process_Com(uint8_t id, uint8_t * b)
{
  int x = 0;
  int y = 0;
  int g = 0;
	if (id != 200) {
		if(id != 7)
			this->net->send_full(id+48,b[0]+48,b[1]+48,b[2]+48,b[3]+48,b[4]+48,'\n');

              switch(id) {
         
					case 2 : // Stop frame
						this->controllerState = Idle;
						break;
					case 3 : // Move frame
						this->controllerState = Manual;
						break;
					case 4 : // Move + Acq frame
						// this->controllerState = Manual_Acquisition;
						break;
					case 5 : // Move + Acq frame
						this->controllerState = Manual_Acquisition;
						break;
					case 6 : // Scanning
						this->controllerState = Scan;
						break;
					case 7 : // Request for acquisition
						// Rechange the data :
						x = this->acq->get_MoveX();
						y = this->acq->get_MoveY();
                                                g = this->imu->getGValue();
						if (x < 0) x = 255-x;
						if (y < 0) y = 255 - y;
						// Send packet ...
                                                // x = 100;
                                                // y = 100;
						this->net->send_data_packet(x,y,g);
						break;
					default :
						this->controllerState = Idle;
						break; 
          
                  }
	
    	}
}

void Controller ::  Process_Acq()
{
	// ACQUISITION UPDATE
	this->acq->run_the_magic(); // update values...
        this->imu->run_the_magic();
	this->acq_system++;
	// Process PID
      /*if(this->acq_system%2 == 0)
		// PID ADNS
		delay(4);
	else
                delay(4);
		// PID MPU
		// delay(8);*/
}

