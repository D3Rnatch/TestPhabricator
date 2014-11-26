
#include "motor_handler.h"


	/** \brief Motor_handler 
	*	\param int 0 : min speed
	*	\param int 1 : max speed
	*/
	Motor_handler::	Motor_handler(int m, int a, uint8_t c, uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4)
	{
		this->min = m;
		this->max = a;
		
		this->motor_array[0].attach(c);
		this->motor_array[1].attach(l1);
		this->motor_array[2].attach(l2);
		this->motor_array[3].attach(l3);
		this->motor_array[4].attach(l4);
		
		// INIT OF THE CONTROL MATRICES
		this->power_matrix = { 0,0,0,0,0 };
		this->activation_matrix = { 0,0,0,0,0 };
		
	}
	
	/** \brief Maintains motors at speed
	*
	*/
	void Motor_handler::run_the_magic()
	{
		for(int i=0;i<5;i++)
			this->refresh_motor(i);
	}
	
	void Motor_handler::refresh_motor(int a)
	{
		if(a < 5 && a >= 0)
		{
			if(this->activation_matrix[a]){
				int v = map(this->power_matrix[a], 0, 100, this->min, this->max);
				this->motor_array[a].write(v);
			}
		}
	}
	
	/**	\brief Activate_motor
	* \param int 0 : motor number 
	* \param int 1 : power
	*/
	void Motor_handler::activate_motor(int a, int b)
	{
		if(a < 5 && a >= 0){
			this->activation_matrix[a] = 1;
			this->power_matrix[a] = b%this->max;
		}
	}
	
	void Motor_handler::deactivate_motor(int)
	{
		if(a < 5 && a >= 0){
			this->activation_matrix[a] = 0;
			this->power_matrix[a] = 0;
		}
	}
	
	#ifdef DBG
		void Motor_handler::test_procedure()
		{
			// Test Lateral motors
			for(int i=1;i<5;i++)
			{
				this->activate_motor(i, TEST_RATE_0);
				this->refresh_motor(i);
			}
			delay(10);
			for(int i=1;i<5;i++)
			{
				this->activate_motor(i, TEST_RATE_MID);
				this->refresh_motor(i);
			}
			delay(10);
			for(int i=1;i<5;i++)
			{
				this->activate_motor(i, TEST_RATE_HIGH);
				this->refresh_motor(i);
			}
			delay(300);
			
			// Test lift motor
			this->activate_motor(0, TEST_RATE_0);
			this->refresh_motor(0);
			delay(15);
			
			this->activate_motor(0, TEST_RATE_MID);
			this->refresh_motor(0);
			delay(15);
			
			this->activate_motor(0, TEST_RATE_HIGH);
			this->refresh_motor(0);
			delay(300);
		}
	#endif
	
	void Motor_handler::update_activation_matrix(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e)
	{
		this->activation_matrix[0] = a;
		this->activation_matrix[1] = b;
		this->activation_matrix[2] = c;
		this->activation_matrix[3] = d;
		this->activation_matrix[4] = e;
	}
	
	void Motor_handler::update_power_matrix(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e)
	{
		this->power_matrix[0] = a;
		this->power_matrix[1] = b;
		this->power_matrix[2] = c;
		this->power_matrix[3] = d;
		this->power_matrix[4] = e;
	}
	
	/**
	*	Stops all motors immediately
	*
	*/
	void Motor_handler::emergency_stop()
	{
		for(int i=0;i<5;i++)
		{
			this->power_matrix[i] = 0;
			this->refresh_motor(i);
			this->deactivate_motor(i);
		}
	}