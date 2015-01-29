
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
 * /!\ WARNING : there is a problem of distance calculation : error range : Dist € [-10%;+10%].
 *
 */
#include <Arduino.h>
#include <math.h>
#include "ADNS2051.h"

// For ADNS driver : Variable read in datasheet of ADNS2610
#define ADNS_FPS 1500 /*!< Frame per second : 2 frames are necessary for detection.*/
#define ADNS_ACQ_SPEED 0.002 /*!< Time minimum of acquisition : 2ms.*/
#define ADNS_F_DIST 1.06 /*!< Focal distance. */
#define INCH_CM 2.54 /*!< Inch towards Cm. */

#define ALPHA 0.00000359 /*!< ALPHA value. */

#define SDIO 50 /*!< SPI protocol wire SDIO Arduino pin. */
#define SCLK 52 /*!< SPI protocol wire SCLK Arduino pin. */
//#define SDIO 3
//#define SCLK 2

#define TRESHOLD_MIN -20 /*!< Min treshold. */
#define TRESHOLD_MAX 20 /*!< Max treshold. */

// #define CALL_MEMBER_FN(object,ptrToMember) ((object).*(ptrToMember))
/**
 * \class ACQ_handler
 * \brief Manage acquisition on the Arduino.
 */
class ACQ_handler
{
	public :
		/**
                * \fn void ACQ_handler()
                * \brief Setup of the mouse sensor.
                *   Start a SPI bus on ICSP pins of the Arduino Mega.
                *   Also test the readings of the optical sens
                */
		ACQ_handler();
		
		/**
                * \fn void run_the_magic()
                * \brief Updates readings of optical mouse sensor.
                */
		void run_the_magic();

                /**
                * \fn int get_MoveX()
                * \brief Returns the actual stack of reading of the optical sensor.
                *     Returns the actual stack of reading of the optical sensor. Empties the register on reading.
                * \return stacked register value read (NEXT IMPLEMENTED : distance made by mobile).
                */		
                int get_MoveX();
		
                /**
                * \fn int get_MoveY()
                * \brief Returns the actual stack of reading of the optical sensor.
                *     Returns the actual stack of reading of the optical sensor. Empties the register on reading.
                * \return stacked register value read (NEXT IMPLEMENTED : distance made by mobile).
                */int get_MoveY();

		/**
                * \fn int get_MoveXY()
                * \brief Returns the actual distance on both axis.
                *     Returns the actual distance on both axis. Empties the register on reading.
                * \return stacked distance value read (NEXT IMPLEMENTED : the actual reading).
                */
		double get_MoveXY();

		/**
                * \fn int get_MoveAngle()
                * \brief Outdated
                *     -
                * \return -
                */
		double get_MoveAngle();

		/**
                * \fn void update_values()
                * \brief Returns the actual distance on both axis.
                *     Returns the actual distance on both axis. Empties the register on reading.
                * \return stacked distance value read (NEXT IMPLEMENTED : the actual reading).
                */		
                void update_values();

		// For lib PID synchronization.
		/** \brief Mouse X speed measurement (RAW).*/
		int delta_x_adns; 
		/** \brief Mouse Y speed measurement (RAW).*/
		int delta_y_adns;
		/** \brief Counter reaction of sustentation's EDF.*/
		int g_z_mpu;


		////////////////////////////////////////////:
		//		ADNS ZONE
		////////////////////////////////////////////:
		// Object :
		ADNS2051 optical1;

		int ADNSgetFilteredValueY();
		
		int ADNSgetFilteredValueX();
	
	private :

		byte frame[FRAMELENGTH];

		byte flop;

		double actual_r; // r
		double actual_theta; // deg
		int last_x;
		int last_y;


};


#endif // ACQ_HANDLER_H

