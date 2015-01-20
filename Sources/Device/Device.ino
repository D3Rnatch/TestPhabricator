
/**
 * \file Arduino_Code.ino
 * \brief Main arduino code.
 * \author Alexandre Brand
 * \version 1.0
 *
 * Main file containing starting point for Arduino code.
 *
 */

#include <Arduino.h>
#include <Servo.h>
#include "controller_helper.h"

uint8_t cpt = 0;
Controller *controller;
/**
 * \fn void setup()
 * \brief Setup function for the Arduino.
 */
void setup()
{
        controller = new Controller();
	controller->init();
}

/**
 * \fn void loop()
 * \brief Loop function for the arduino.
 */
void loop()
{
	controller->net->run_the_magic();
		
	// resend data
	uint8_t id = (byte)controller->net->get_last_frame_id();
	uint8_t * b = controller->net->get_array();
		
	// Processing Communication
	controller->Process_Com(id, b);	
	
	// If controllerState is : 
	if(controller->controllerState == Manual_Acquisition) {
		controller->Process_Acq();
		if(id == 3)
			controller->Process_Motor(b);
	}

	if(controller->controllerState == Manual) {
		if(id == 3)
			controller->Process_Motor(b);
	}

	if(controller->controllerState == Scan) {
		controller->Process_Scan(b);
	}

	if(controller->controllerState == Automatic) {
		// TO BE IMPLEMENTED
	}

	if(controller->controllerState == Idle) {
		// TO BE IMPLEMENTED
	}
}


