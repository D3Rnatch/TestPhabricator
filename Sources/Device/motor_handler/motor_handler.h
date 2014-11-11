
#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include "servo.h"

#define DBG

// Maximum rate value given to motors
// in %
#define MIN 0
#define MAX 60

// Test rate
#define TEST_RATE_0 MIN
#define TEST_RATE_MID ((MAX - MIN)/2)
#define TEST_RATE_HIGH MAX

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
	
	/** \brief Maintains motors at speed
	*
	*/
	void run_the_magic();
	
	/**	\brief Activate_motor
	* \param int 0 : motor number 
	* \param int 1 : power
	*/
	void activate_motor(int, int);
	
	void deactivate_motor(int);
	
	#ifdef DBG
		void test_procedure();
	#endif
	
	void update_activation_matrix(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
	void update_power_matrix(uint8_t ,uint8_t ,uint8_t ,uint8_t ,uint8_t );
	
	/**
	*	Stops all motors immediately
	*
	*/
	void emergency_stop();
	
	private :
	
	void refresh_motor(int);
	
	Servo motor_array[5];
	uint8_t activation_matrix[5];
	uint8_t power_matrix[5];
	uint8_t min;
	uint8_t max;
};

#endif