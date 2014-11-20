/*  /!\Arduino 
	CLOSED LOOP CONTROL MAIN PROGRAM : Eq. Autopilot
	For Hovercraft using 4 lateral motors
*/

#include <Arduino.h>

// Perso libraries
#include "network_manager.h"
#include "pid_manager.h"
#include "scheduler.h"
#include "motor_handler.h"
#include "acq_handler.h"

#define DBG

typedef struct control
{
	Motor_handler *motor;
	PID_manager *pid;
	Network_manager *net;
	Scheduler *scheduler;
	ACQ_handler *acq;
}control_t;

control_t controller;

void setup()
{
	// Network init
	controller.net = new Network_manager(9600);

	// Motors Init
	controller.motor = new Motor_handler(0,180,0,2,4,6,8);

	// ACQ Handler init
	controller.acq = new ACQ_handler();

	// PID manager Init
	controller.pid = new PID_manager();

	// Scheduler
	controller.scheduler = new Scheduler();

	// Send OK ready signal to Raspberry PI
	controller.net->send_data_to_computer(0xFF); // Sends  : all set !
	#ifdef DBG
		controller.net->send_dbg_message_to_computer("Init finished.");
	#endif
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
	// Network receivings..
	controller.net->run_the_magic();	

	// Process orders...
	Process_Message();	

	// ACQ process...
	controller.acq->run_the_magic();

	// Process the ACQs...
	Process_Acq();

	// PID manager process...
	controller.pid->run_the_magic();

	// Compute new motor entries...
	Proccess_Pid();

	// Updates motor rate speed...
	controller.motor->run_the_magic();

	controller.scheduler->regulate_loop();
}



