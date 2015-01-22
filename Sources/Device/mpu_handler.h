
#ifndef MPU_Handler_H
#define MPU_Handler_H
// #include <Arduino.h>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
// #include <I2Cdev.h>
// #include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file
/*
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif
*/
class MPU6050;

class MPU_Handler
{
	public :
		MPU_Handler();
		
		void run_the_magic();
		
		bool isDisponible();
		
		void calibrate_IMU();
		
		int getGValue();

                static void dmpDataReady();

                // Error : 12 <=> WARNING IMU not connected
                // Error : 22 <=> WARNING IMU dmp not reachable
                int getErrorCode();

		int treshold;
		float mean;
		int g_value;
		
	private :
		
		MPU6050 *mpu;
		float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
		VectorFloat gravity;    // [x, y, z]            gravity vector
		Quaternion q;           // [w, x, y, z]         quaternion container
		// MPU control/status vars
		uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
		uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
		uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
		uint16_t fifoCount;     // count of all bytes currently in FIFO
		uint8_t fifoBuffer[64]; // FIFO storage buffer
                bool isConnected;
                bool dmpReady;  // set true if DMP init was successful

};


#endif
