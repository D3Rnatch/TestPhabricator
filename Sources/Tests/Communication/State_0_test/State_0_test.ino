/*  /!\Arduino 
	CLOSED LOOP CONTROL MAIN PROGRAM : Eq. Autopilot
	For Hovercraft using 4 lateral motors
*/

#include <Arduino.h>

// Perso libraries
#include "protocol_helper.h"
#include "network_manager.h"
#include "motor_handler.h"

#define DBG

typedef struct control
{
	Motor_handler *motor;
	Network_manager *net;
	bool start;
	uint8_t state;
}control_t;

control_t controller;

t_encap sending_frame;

void Control_Process_Com();
void Control_Process_Acq();
void Control_Process_MotorEncoder();
void Control_Process_Decision();

void setup()
{
	// Static inits
	controller.start = false;
	controller.state = -1; // No state

	// Network init
	// controller.net = new Network_manager(9600);
	Serial.begin(9600);

	// Motors Init
	controller.motor = new Motor_handler(0,180,0,2,4,6,8);

	// Send OK ready signal to Raspberry PI
	// controller.net->send_data_to_computer(0xFF); // Sends  : all set !
	// Send handcheck : frame : controller Ready
	Serial.flush();
	sending_frame = encap_ready();
	Serial.write(sending_frame.array,5);
}


///// LOOP IS CONSTRUCTED AS FOLLOW :
/*
	Time :
Div1 : ACQ process, readings taken from ADNS2610 & MPU6050 (12ms)
Div1 : Motor Update : we need to continuously reset the motor process
Div2 : PID computation (every 24 ms) : ensures stability
Div1 : Network communication : send and receive orders. Ensures safety. 
*/
void loop()
{

	// Network Manager
	// controller.net->run_the_magic();
	Control_Process_Com();	
	
	while(controller.start && controller.state != -1)
	{
		Serial.print("OKKO\n");

		// Com reading
		// controller.net->run_the_magic();
		Control_Process_Com();
		
		// Up motors
		controller.motor->run_the_magic();		

		// State gestionary
		stateGestionary();

		// Up motors
		controller.motor->run_the_magic();

		// Time loop control
		delay(30);
	}
}

void Control_Process_Com()
{
	// Get the latest com frame
	// char *t = controller.net->read_data_from_buffer();
	// extracting data from id ....
	// t_recept_encap e = extract_data_bytes((byte*)t);
	uint8_t value[7]; 
        Serial.readBytes((char*)value, 7);
	Serial.flush();       
	
	t_frame_bytes e = extract_data_bytes((byte*)value);
	switch(e.type)
	{
		// It is a state and start signal
		case 0 :
			controller.state = (uint8_t)e.array[1];
			controller.start = (uint8_t)e.array[2];
		break;
		// Stop frame
		case 1 :
			controller.start = 0;
		break;
		// Move call...
		case 2 :
			if((controller.state ==0 || controller.state == 1 )&&(controller.start == 1)) {
			controller.motor->update_activation_matrix(e.array[0]==0?0:1,e.array[1]==0?0:1,e.array[2]==0?0:1,e.array[3]==0?0:1,1);
			controller.motor->update_power_matrix(e.array[0],e.array[1],e.array[2],e.array[3],30); }
		break;
	}

	// When processed : resend frame. For DBG...
	// for(int i=0;i<10;i++)
		Serial.write(value,7);
}

void Control_Process_Acq()
{
	// TO BE IMPLEMENTED
}

void Control_Process_MotorEncoder()
{
	// TO BE IMPLEMENTED
}

void Control_Process_Decision()
{
	// TO BE IMPLEMENTED
}

/**
*	\Brief stateGestionary : real main loop
*
*/
void stateGestionary()
{
	switch(controller.state)
	{
		case 1 : // Manual + PID
			// ACQ process...
			// controller.acq->run_the_magic();
			Control_Process_Acq();

			// Up motors
			controller.motor->run_the_magic();

			// PID manager process...
			// controller.pid->run_the_magic();
			Control_Process_MotorEncoder();		
		break;

		case 0 : // Manual Pure
			// Not usefull at the moment
		break;	

		case 2 : // Autonomous Process
			
			// ACQ process...
			//controller.acq->run_the_magic();
			Control_Process_Acq();

			// Up motors
			controller.motor->run_the_magic();

			// PID manager process...
			//controller.pid->run_the_magic();
			Control_Process_Decision();
			Control_Process_MotorEncoder();	

			// Up motors
			controller.motor->run_the_magic();			
		break;

		default :
		break;
	}
}


