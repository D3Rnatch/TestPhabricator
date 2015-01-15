#ifndef CONTROLLER_HELPER
#define CONTROLLER_HELPER

#include "network_manager.h"
#include "acq_handler.h"

#define MIN 10
#define MAX 90

enum State
{
   Idle,
   Manual,
   Manual_Acquisition,
   Scan,
   Automatic
};

typedef struct struct_controller
{
	// Servo :
	Servo escenter[4];
	Servo LaserScaner;
	
	// State :	
	State controllerState;

	// Services
	Network_manager *net;
	ACQ_handler *acq;

	unsigned long time;
	unsigned long last_time;
	boolean motor_last_set = false;
	uint8_t motor_off_cpt = 0;
	boolean scaner_last_set = false;
	uint8_t scaner_off_cpt = 0;
}Controller_t;

Controller_t Controller;

void setSpeedcent(int a, int spd);
void setSpeedFans(uint8_t * b);
void setAngleLaserScaner(uint8_t angle);
void calibrate_laserscaner();
void arm();

void Process_Motor(uint8_t * b);
void Process_Scan(uint8_t * b);
void Process_Acq();
void reset_Services();
void Process_Com(uint8_t id, uint8_t * b);

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
* //////	
* //////	
* //////	
* //////	
* //////		**/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	        // net->send(b[0]+48,b[1]+48,b[2]+48,b[3]+48,b[4]+48,'\n');        
	  Controller.net->send_full(id,b[0],b[1],b[2],b[3],b[4],'\n');
	  
	  if (id == 1){
		switch(b[0])
		{
			case 0 : // Idle State
				Controller.controllerState = Idle;
			break;
			case 1 : // Manual State
				Controller.controllerState = Manual;
			break;
			case 2 : // ACQ_MANUAL state
				Controller.controllerState = Manual_Acquisition;
			break;
			case 3 : // Automatic
				Controller.controllerState = Automatic;
			break;
		}
	  }
	  	
    	}
}

void Process_Acq()
{

}

#endif
