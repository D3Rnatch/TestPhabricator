#ifndef CONTROLLER_HELPER
#define CONTROLLER_HELPER

/**
 * \file controller_helper.h
 * \brief Main controller functions.
 * \author Alexandre Brand
 * \version 1.0
 */
#include "network_manager.h"
#include "acq_handler.h"

#define MIN 10
#define MAX 90

/**
 * \enum enum State
 * \brief State enumerator.
 *
 * Describes the robot state.
 *
 */
enum State
{
   Idle, /*!< Waiting mode. */
   Manual, /*!< Manual mode. */
   Manual_Acquisition, /*!< Manual aquisition mode. */
   Scan, /*!< Scanning mode. */
   Automatic /*!< Auto mode. */
};

/**
 * \struct Controller_t
 * \brief Strore everything for the robot.
 */
typedef struct struct_controller
{
	// Servo :
	Servo escenter[4];/*!< Servo speed map. */
	Servo LaserScaner;/*!< Scanner servo. */
	
	// State :	
	State controllerState;/*!< Current state save. */

	// Services
	Network_manager *net;/*!< Network service */
	ACQ_handler *acq;/*!< Acquisition service */

	unsigned long time;/*!< Time save */
	unsigned long last_time;/*!< Save last time */
	boolean motor_last_set = false;/*!< Is motors on ? */
	uint8_t motor_off_cpt = 0;/*!< Motor off since. */
	boolean scaner_last_set = false;/*!< Is scanner on ? */
	uint8_t scaner_off_cpt = 0;/*!< Scanner on since. */
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

//////////////////////////////////
//				//
//	DEFINITIONS		//
//				//
//////////////////////////////////
/**
 * \fn void setSpeedcent(int a, int spd)
 * \brief Set desired speed.
 * \param a Motor number.
 * \param spd Desired speed.
 */
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
