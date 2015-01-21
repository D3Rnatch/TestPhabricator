
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
#include <math.h>

// #include "ADNS2610_arduino_driver.h"


// #include "MPU6050_arduino_driver.h"

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

#define DLEDG 9
#define DLEDR 10
#define DLEDY 11
#define DLEDPERF 13

#define FRAMELENGTH 324


		const byte regConfig    = 0x00;
		const byte regStatus    = 0x01;
		const byte regPixelData = 0x08;
		const byte maskNoSleep  = 0x01;
		const byte maskPID      = 0xE0;
		const byte regDeltaX    = 0x02;
		const byte regDeltaY    = 0x03;
		const byte regSqual     = 0x04;


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
		int get_MoveX();
		/** \brief Returns the y distance since last acq in cm.*/
		int get_MoveY();

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


		////////////////////////////////////////////:
		//		ADNS ZONE
		////////////////////////////////////////////:
		/**
		 * \fn void ADNSmouseInit(void)
		 */
		void ADNSmouseInit(void);
		/**
		 * \fn void ADNSdumpDiag(void)
		 */
		void ADNSdumpDiag(void);
		/**
		 * \fn void ADNSwriteRegister(byte addr, byte data)
		 */
		void ADNSwriteRegister(byte addr, byte data);
		/**
		 * \fn void ADNSreadRegister(byte addr)
		 */
		byte ADNSreadRegister(byte addr);
		/**
		 * \fn void ADNSreadFrame(byte *arr)
		 * \brief ADNS2610 dumps a 324-byte array, so this function assumes arr points to a buffer of at least 324 bytes.
		 */
		void ADNSreadFrame(byte *arr);
		/**
		 * \fn void ADNSflash(byte pin, byte nTimes)
		 */
		void ADNSflash(byte pin, byte nTimes);
		/**
		 * \fn void ADNSflipLED(void)
		 */
		void ADNSflipLED(void);

		// HIGH LEVEL FUNCTIONS
		/**
		 * \fn void ADNSgetTrueValue(byte data)
		 * \brief Allows negative values.
		 */
		int ADNSgetTrueValue(byte data);
		/**
		 * \fn void ADNSTresholdFilter(int data)
		 */
		int ADNSTresholdFilter(int data);
		/**
		 * \fn void ADNSgetFilteredValueY()
		 */
		int ADNSgetFilteredValueY();
		/**
		 * \fn void ADNSgetFilteredValueX()
		 * \brief Returns DeltaX value using the ThresholdFilter.
		 */
		int ADNSgetFilteredValueX();
		/**
		 * \fn void ADNSgetSqual()
		 */
		int ADNSgetSqual();
		
	
	private :

		byte frame[FRAMELENGTH];

		byte flop;

		double actual_r; // r
		double actual_theta; // deg
		int last_x;
		int last_y;
/*
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
*/
                // void dmpDataReady();
		void mpuCalibrate();
		void acquire_mpu();

};


#endif // ACQ_HANDLER_H

