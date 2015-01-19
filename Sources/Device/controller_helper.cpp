#include "controller_helper.h"


//////////////////////////////////
//				//
//	DEFINITIONS		//
//				//
//////////////////////////////////

void setSpeedcent(int a, int spd){
  //On envoie la vitesse désirée
  int vit = map(spd, 0, 100, 0, 180);
  Controller.escenter[a].write(vit);
}

void setSpeedFans(uint8_t * b)
{
  int vit[4]; 
 for(int i=0;i<4;i++) {
     if(b[i]>MAX) b[i] = MAX;
     else if (b[i] < MIN) b[i] = MIN;
     vit[i] = map(b[i],0,100,0,180);
  }
 for(int j=0;j<4;j++)
     Controller.escenter[j].write(vit[j]);  
}

void setAngleLaserScaner(uint8_t angle)
{
	// int vit = map(angle,0,180,1,179);
	Controller.LaserScaner.write(angle);
}

void calibrate_laserscaner()
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

void arm(){
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

void Process_Motor(uint8_t * b)
{
	if (Controller.motor_last_set == false) Controller.motor_last_set = true;
		       
        // we set fans power.
        setSpeedFans(b);
                
        for(int i=0;i<4;i++) {
               b[i] = 0;
        }
}

void Process_Scan(uint8_t * b)
{
	if (Controller.scaner_last_set == false){
		 // Setting to point 0 the scaner
		 // setAngleLaserScaner(0);		
		 Controller.scaner_last_set = true;
	}
	setAngleLaserScaner (b[0]);
	for(int i=0;i<4;i++) {
		b[i] = 0;
	}

}

void reset_Services()
{
	// TO BE IMPLEMENTED

	switch(Controller.controllerState)
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

void Process_Com(uint8_t id, uint8_t * b)
{
	if (id != 200) {
		if(id != 7)
			Controller.net->send_full(id,b[0],b[1],b[2],b[3],b[4],'\n');
		switch(id)
		{
			case 1:
				switch(b[0])
				{
					case 0 : // Idle State.
						Controller.controllerState = Idle;
						break;
					case 1 : // Manual State.
						Controller.controllerState = Manual;
						break;
					case 2 : // ACQ_MANUAL state.
						Controller.controllerState = Manual_Acquisition;
						break;
					case 3 : // Automatic.
						Controller.controllerState = Automatic;
						break;
					defafult: //Idle State by default.
						Controller.controllerState = Idle;
				}
				break;
	  	}  	
    	}
}

void Process_Acq()
{

}

