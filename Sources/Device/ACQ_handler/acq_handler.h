
#ifndef ACQ_HANDLER_H
#define ACQ_HANDLER_H

#include <Arduino.h>

#include "math.h"

#include "ADNS2610_arduino_driver.h"
#include <I2Cdev.h>
#include <helper_3dmath.h>
#include <MPU6050_6Axis_MotionApps20.h>

// #include "MPU6050_arduino_driver.h"

// For ADNS driver : Variable read in datasheet of ADNS2610
#define ADNS_FPS 1500 // Frame per second : 2 frames are necessary for detection.
#define ADNS_ACQ_SPEED 0.002 // Time minimum of acquisition : 2ms
#define ADNS_F_DIST 1,06 // Focal distance
#define INCH_CM 2,54 // Inch towards Cm

// In case of problematic calculation
// for F_DIST = 1,06
// Alpha is the number permitting the transition between : Dx (Range : -128 ; 127) towards a distance value in cm.
// /!\ WARNING : there is a problème of distance calculation : error range : Dist € [-10%;+10%].
#define ALPHA 0.00000359

// #define CALL_MEMBER_FN(object,ptrToMember) ((object).*(ptrToMember))
class ACQ_handler
{
	public :
		// Constructor
		ACQ_handler();
		
		// Updates every values
		void run_the_magic();

		// Returns the distance since last acq in cm.
		double get_MoveX();
		double get_MoveY();

		// Returns the exact distance since last acq in cm
		double get_MoveXY(); // getR
		double get_MoveAngle(); // getO
		void update_values();

		// For lib PID synchronization.
		int delta_x_adns; // Mouse X speed measurement (RAW)
		int delta_y_adns; // Mouse Y speed measurement (RAW)
		int g_z_mpu; // Counter reaction of sustentation's EDF

	private :
		double actual_r; // r
		double actual_theta; // deg
		double last_x;
		double last_y;

		MPU6050 *mpu;
		boolean mpuInterrupt;
		// boolean dmpReady;
		uint8_t mpuIntStatus;
		uint8_t packetSize;
		uint8_t devStatus;
		// Quaternion
		Quaternion q;
		// euler results
		float euler[3];
		// ypr results
		float ypr[3];
                // Gravity vector
                VectorFloat gravity;

		uint8_t errorCode;

                uint8_t fifoBuffer[64]; // FIFO storage buffer
                uint16_t fifoCount;     // count of all bytes currently in FIFO

                // void dmpDataReady();
		void mpuCalibrate();
		void acquire_mpu();
};


#endif // ACQ_HANDLER_H

