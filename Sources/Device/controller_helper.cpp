#include "controller_helper.h"


Controller :: Controller()
{
	this->net = new Network_manager(9600);

	this->escenter[0].attach(2);
    	this->escenter[1].attach(4);
    	this->escenter[2].attach(6);
    	this->escenter[3].attach(8);
	this->LaserScaner.attach(3);
        // Serial.println("OK esc.");
	// Starting The Acq manager
	this->acq = new ACQ_handler();
        // Serial.println("End OF INIT");
}

void Controller :: init()
{


	this->calibrate_laserscaner();
    	
        this->arm();

	this->controllerState = Idle;

	// Controller finished init 
	// Sending OK ready packet.

        // Serial.print("TEST");
	this->net->send_ready_packet(0);

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
	if (id != 200) {
		if(id != 7)
			this->net->send_full(id,b[0],b[1],b[2],b[3],b[4],'\n');
		switch(id)
		{
			case 1:
				switch(b[0])
				{
					case 0 : // Idle State.
						this->controllerState = Idle;
						break;
					case 1 : // Manual State.
						this->controllerState = Manual;
						break;
					case 2 : // ACQ_MANUAL state.
						this->controllerState = Manual_Acquisition;
						break;
					case 3 : // Automatic.
						this->controllerState = Automatic;
						break;
					default: //Idle State by default.
						this->controllerState = Idle;
				}
				break;
	  	}  	
    	}
}

void Controller ::  Process_Acq()
{

}

