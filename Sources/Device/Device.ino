
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
#include <I2Cdev.h>
#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include "PI_lib.h"
// #include "mpu_handler.h"

#include "controller_helper.h"

uint8_t cpt = 0;
Controller *controller;
/**
 * \fn void setup()
 * \brief Setup function for the Arduino.
 */
// MPU_Handler * imu;
void setup()
{
        // join I2C bus (I2Cdev library doesn't do this automatically)
        #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
            Wire.begin();
            TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
        #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
            Fastwire::setup(400, true);
        #endif

       controller = new Controller(); 
       controller->init();
      //Serial.println("End of Init.");
}

/**
 * \fn void loop()
 * \brief Loop function for the arduino.
 */
uint8_t prec = 0;
uint8_t actual = 0;
void loop()
{
        actual = millis();
        
        // Run the IMU unstack process
        // And the ADNS acquisition.
        controller->imu->run_the_magic();
        controller->acq->run_the_magic();
        
        // Run the Serial Manager : Gets the computer's entries.
        controller->net->run_the_magic();
        
	// Extract frame's id.
	uint8_t id = (byte)controller->net->get_last_frame_id();
	// Extract frame's byte array
        uint8_t * b = controller->net->get_array();
		
	// Processing Communication : switching state of autopilot
	controller->Process_Com(id, b);	

        // On manual and Acquisition state : Process the ACQ (get the distance calculated) and the Motor update.
	if(controller->controllerState == Manual_Acquisition) {
                  controller->reset_Services();
		controller->Process_Acq();
		if(id == 3)
			controller->Process_Motor(b);
	}
        
        // On manual only Motors are updated
	if(controller->controllerState == Manual) {
                  controller->reset_Services();
		if(id == 3)
			controller->Process_Motor(b);
	}
        
        // On scan mode we process the environment scan process
	if(controller->controllerState == Scan) {
                controller->reset_Services();
		controller->Process_Scan(b);
	}

        // TO BE IMPLEMENTED
	if(controller->controllerState == Automatic) {
                  controller->reset_Services();
		// TO BE IMPLEMENTED
	}

        // On Idle mode : reset of services and idle state for controller.
	if(controller->controllerState == Idle) {
                controller->reset_Services();
		// TO BE IMPLEMENTED
	}

        prec = actual;
        actual = millis();
}


