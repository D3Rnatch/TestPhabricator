
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
       //Serial.println("END OF SETUP. GOING MAIN LOOP");
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
        
        controller->imu->run_the_magic();
        // while(!Serial.available()) Serial.println("attente");
        // Serial.println("AVANT NET");
	//controller->net->run_the_magic();
       // Serial.println("APRES NET");
       // imu->run_the_magic();
        
        //Serial.print(" G value is : ");
        //Serial.println(controller->imu->getGValue(),DEC);
       // Serial.println(imu->getGValue(),DEC);
        
	// resend data
	uint8_t id = (byte)controller->net->get_last_frame_id();
	uint8_t * b = controller->net->get_array();
		
	// Processing Communication
	controller->Process_Com(id, b);	
	
        ////Serial.print("Attention : ");
        ////Serial.println(id,DEC);

	// If controllerState is : 
	if(controller->controllerState == Manual_Acquisition) {
                ////Serial.println("MANUAL_ACQ MODE");
		controller->Process_Acq();
		if(id == 3)
			controller->Process_Motor(b);
	}

	if(controller->controllerState == Manual) {
                ////Serial.println("MANUAL_ACQ MODE");
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
                ////Serial.println("Idle State.");
		// TO BE IMPLEMENTED
	}// */

        prec = actual;
        actual = millis();
        //Serial.print("Time loop is : ");
        //Serial.println(actual-prec,DEC);
}


