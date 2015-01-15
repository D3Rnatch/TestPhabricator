
#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include "servo.h"

#define DBG

// Maximum rate value given to motors
// in %
#define MIN 10
#define MAX 90

// Test rate
#define TEST_RATE_0 MIN
#define TEST_RATE_MID ((MAX - MIN)/2)
#define TEST_RATE_HIGH MAX

#define LAT1 2
#define LAT2 4
#define LAT3 6
#define LAT4 8
#define LAT5 10

class Motor_handler
{
	public :
	


	/** \brief Motor_handler 
	*	\param int 0 : min speed
	*	\param int 1 : max speed
	*	\param uint8_t : attached pin 0 : ground motor.
	* 	\param uint8_t x : HL, HR, DL, DR
	*/
	Motor_handler(int, int, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
	Motor_handler();
	
	/** \brief Maintains motors at speed
	*
	*/
	void run_the_magic();

	/**
	*	Stops all motors immediately
	*	Almost, Almost
	*/
	void emergency_stop();

	void setSpeedFans(uint8_t *);
	
	private :
	
	void arm();

	Servo motor_array[5];
	uint8_t min;
	uint8_t max;
	uint8_t f[5];
	boolean on_stop;
};

#endif
