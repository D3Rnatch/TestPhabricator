
#include "motor_handler.h"


	Motor_Handler :: Motor_Handler()
	{
		motor_array[0].attach(LAT1);
		motor_array[1].attach(LAT2);
		motor_array[2].attach(LAT3);
		motor_array[3].attach(LAT4);
		motor_array[4].attach(LAT5);

		this->min = MIN;
		this->max = MAX;

		for(int i=0;i<5;i++)
			this->f[i] = MIN;

		this->arm();
	
		this->on_stop = false;
	}


	Motor_Handler :: Motor_Handler (int m, int mm, uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4, uint8_t t5)
	{
		
		// TO BE IMPLEMENTED
		
	}

	void Motor_Handler :: arm()
	{
		setSpeedcent(0,MIN);
  		setSpeedcent(1,MIN);
  		setSpeedcent(2,MIN);
  		setSpeedcent(3,MIN);
  		setSpeedcent(4,MIN);
  		delay(1200);
  		setSpeedcent(0,MAX);
		setSpeedcent(1,MAX);
  		setSpeedcent(2,MAX);
  		setSpeedcent(3,MAX);
  		setSpeedcent(4,MAX);
  		delay(1200);
  		setSpeedcent(0,MIN);
  		setSpeedcent(1,MIN);
  		setSpeedcent(2,MIN);
  		setSpeedcent(3,MIN);
  		setSpeedcent(4,MIN);
  		delay(1200);		
	}

	void Motor_Handler :: setSpeedFans(uint8_t * b)
	{
		int vit[5];
		for(int i=0;i<5;i++)
		{
			if(b[i]>MAX) b[i] = MAX;
			else if(b[i]<MIN) b[i] = MIN;
			this->f[i] = b[i];
			vit[i] = map(this->f[i],0,100,0,180);
		}
		for(int j=0;j<5;j++)
			motor_array[j].write(vit[j]);
	}

	// Permet un arrêt immédiat des moteurs.
	// cet arrêt est :
	// 1 -> decrescendo.
	// Autre -> brutal.
	void Motor_Handler :: emergency_stop(uint8_t type)
	{
		if(this->on_stop == false) {
			if(type) {
				this->on_stop = true;
			}
		}
	}



