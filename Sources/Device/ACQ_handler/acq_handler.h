
#ifndef ACQ_HANDLER_H
#define ACQ_HANDLER_H

#include "math.h"

#include "ADNS2610_arduino_driver.h"
#include "MPU6050_arduino_driver.h"

// For ADNS driver : Variable read in datasheet of ADNS2610
#define ADNS_FPS 1500 // Frame per second : 2 frames are necessary for detection.
#define ADNS_ACQ_SPEED 0.002 // Time minimum of acquisition : 2ms
#define ADNS_F_DIST 1,06 // Focal distance
#define INCH_CM 2,54 // Inch towards Cm

// In case of problematic calculation
// for F_DIST = 1,06
// Alpha is the number permitting the transition between : Dx (Range : -128 ; 127) towards a distance value in cm.
// /!\ WARNING : there is a problème of distance calculation : error range : Dist € [-10%;+10%].
#define ALPHA 0,00000359

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
};


#endif // ACQ_HANDLER_H

