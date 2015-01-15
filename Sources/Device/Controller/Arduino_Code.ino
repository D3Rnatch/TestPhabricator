/////////////////////////////////////////////////////////////////////////////
//
//	TO BE DEBUGGUED
//
/////////////////////////////////////////////////////////////////////////////


#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <helper_3dmath.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "controller_helper.h"

// int frame_counter = 

void setup()
{
	Controller.escenter[0].attach(2);
    	Controller.escenter[1].attach(4);
    	Controller.escenter[2].attach(6);
    	Controller.escenter[3].attach(8);
	
	Controller.LaserScaner.attach(3);
        delay(2);
	calibrate_laserscaner();
    	
        arm();

	// Starting The Acq manager
	Controller.acq = new ACQ_handler();

	Controller.net = new Network_manager(9600);
        // Serial.println("End OF INIT");

	Controller.controllerState = Idle;

	// Controller finished init 
	// Sending OK ready packet.
	Controller.net->send_ready_packet(0);
}

uint8_t cpt = 0;
void loop()
{
	Controller.net->run_the_magic();
		
	// resend data
	uint8_t id = (byte)Controller.net->get_last_frame_id();
	uint8_t * b = Controller.net->get_array();
		
	// Processing Communication
	Process_Com(id, b);	
	
	// If controllerState is : 
	if(Controller.controllerState == Manual_Acquisition) {
		Process_Acq();
		Process_Motor(b);
	}

	if(Controller.controllerState == Manual) {
		Process_Motor(b);
	}

	if(Controller.controllerState == Scan) {
		Process_Scan(b);
	}

	if(Controller.controllerState == Automatic) {
		// TO BE IMPLEMENTED
	}

	if(Controller.controllerState == Idle) {
		// TO BE IMPLEMENTED
	}
}


