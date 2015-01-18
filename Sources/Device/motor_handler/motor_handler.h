#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

/**
 * \file motor_handler.h
 * \brief handle motors.
 * \author Alexandre Brand
 * \version 1.0
 */

#include "servo.h"

#define DBG

// Maximum rate value given to motors
// in %
#define MIN 10 /*!< Minimum rate value given to motors in percent. */
#define MAX 90 /*!< Maximum rate value given to motors in percent. */

// Test rate
#define TEST_RATE_0 MIN
#define TEST_RATE_MID ((MAX - MIN)/2)
#define TEST_RATE_HIGH MAX

#define LAT1 2
#define LAT2 4
#define LAT3 6
#define LAT4 8
#define LAT5 10

/**
 * \class Motor_handler
 * \brief Motor management class.
 */
class Motor_handler
{
	public :
	
	/** \brief Motor_handler constructor.
	*   \param m Min speed.
	*   \param mm Max speed.
	*   \param t1 Attached pin 0 : ground motor.
	*   \param t2 HL.
	*   \param t3 HR.
	*   \param t4 DL.
	*   \param t5 DR.
	*/
	Motor_handler(int m, int mm, uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4, uint8_t t5);
	/** \brief default constructor.*/
	Motor_handler();
	
	/** \brief Maintains motors at speed
	*
	*/
	void run_the_magic();

	/**
	*   \brief Stops all motors immediately
	*   \param type Stop type.
	*
	*   type = 1 -> decrescendo.
	*   Other -> brutal.
	*   Almost, Almost.
	*/
	void emergency_stop(uint8_t type);

	/**
	*   \brief Set motor speed.
	*   \param b Speed map.
	*/
	void setSpeedFans(uint8_t * b);
	
	private :
	
	void arm();

	Servo motor_array[5];
	uint8_t min;
	uint8_t max;
	uint8_t f[5];
	boolean on_stop;
};

#endif
