
#include "acq_handler.h"

	ACQ_handler :: ACQ_handler()
	{
		mouseInit(); // Démarrage du capteur optique.
		mpuInit(); // Démarrage du G sensor
		this->delta_x_adns = this->delta_y_adns = 0;
		this->g_z_mpu = 0;
		this->actual_r = this->actual_theta = last_x = last_y = 0.0;		
	}

	void ACQ_handler :: run_the_magic()
	{
		this->delta_y_adns = getFilteredValueY();
		this->delta_x_adns = getFilteredValueX();
		this->g_z_mpu = getFilteredValueGZ(); // /!\ Reading on interrupt only !
		this->update_values();
	}

	void ACQ_handler :: update_values()
	{
		/* Theta Angle */
		int e = ALPHA << 10;
		int xe = this->delta_x_adns * e;
		int ye = this->delta_y_adns * e;
		double x = (double)(xe >> 10);
		double y = (double)(ye >> 10);
		this->last_x = x;
		this->last_y = y;		
		double t = this->actual_theta = atan2(y/x);

		/* Radius Value */
		int cT = (int)(cos(t) << 10);
		int xt = xe;
		if(!cT) {
			cT = (int)(sin(t) << 10);
			xt = ye;
		}
		this->actual_r = (double)((xt/cT) >> 10);
	}

	double ACQ_handler :: get_MoveXY()
	{		
		return this->actual_r; // r	
	}

	double ACQ_handler :: get_MoveAngle()
	{		
		return this->actual_theta; // r	
	}

	double ACQ_handler :: get_MoveX()
	{
		return this->last_x;
	}

	double ACQ_handler :: get_MoveY()
	{
		return this->last_y;
	}
