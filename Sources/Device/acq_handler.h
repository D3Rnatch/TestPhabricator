
#ifndef ACQ_HANDLER_H
#define ACQ_HANDLER_H

/**
 * \file acq_handler.h
 * \brief This files describes acq management.
 * \author Alexandre Brand
 * \version 1.0
 *
 * In case of problematic calculation
 * for F_DIST = 1,06
 * Alpha is the number permitting the transition between : Dx (Range : -128 ; 127) towards a distance value in cm.
 * /!\ WARNING : there is a problème of distance calculation : error range : Dist € [-10%;+10%].
 *
 */
#include <Arduino.h>
//#include <Wire.h>
#include <I2Cdev.h>
#include <helper_3dmath.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <math.h>

#include "ADNS2610_arduino_driver.h"


// #include "MPU6050_arduino_driver.h"

// For ADNS driver : Variable read in datasheet of ADNS2610
#define ADNS_FPS 1500 /*!< Frame per second : 2 frames are necessary for detection.*/
#define ADNS_ACQ_SPEED 0.002 /*!< Time minimum of acquisition : 2ms.*/
#define ADNS_F_DIST 1.06 /*!< Focal distance. */
#define INCH_CM 2.54 /*!< Inch towards Cm. */

#define ALPHA 0.00000359 /*!< ALPHA value. */

// #define CALL_MEMBER_FN(object,ptrToMember) ((object).*(ptrToMember))
/**
 * \class ACQ_handler
 * \brief Manage acquisition on the Arduino.
 */
class ACQ_handler
{
	public :
		/** \brief Constructor.*/
		ACQ_handler();
		
		/** \brief Updates every values.*/
		void run_the_magic();

		/** \brief Returns the x distance since last acq in cm.*/
		double get_MoveX();
		/** \brief Returns the y distance since last acq in cm.*/
		double get_MoveY();

		/** \brief Returns the exact distance (X and Y) since last acq in cm.*/
		double get_MoveXY();
		/** \brief Returns the exact angle since last acq in cm.*/
		double get_MoveAngle();
		/** \brief Updates all values.*/
		void update_values();

		// For lib PID synchronization.
		/** \brief Mouse X speed measurement (RAW).*/
		int delta_x_adns; 
		/** \brief Mouse Y speed measurement (RAW).*/
		int delta_y_adns;
		/** \brief Counter reaction of sustentation's EDF.*/
		int g_z_mpu;

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

