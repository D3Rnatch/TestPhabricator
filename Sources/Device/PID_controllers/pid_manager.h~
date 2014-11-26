/** ARDUINO !!!
*	PID TUNING AND USE LIBRARY
*  This class uses the PID arduino library. It creates an object PID.
* Copyleft PFE Hovercraft 1408 - 2014/2015
*/


#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#include <PID_v1.h>

#define KP 2.0
#define KI 5.0
#define KD 1.0

class PID_manager{
public :
	/**
	* \brief this function creates the array of PID and initialize them.
	* \param :
	*/
	PID_manager(double, double, double);
	PID_manager();
	
	/**
	*
	*
	*/
	void run_the_magic();
	
	/**
	*
	*
	*/
	void compute_selective(int);

	/**
	*	\brief setPID : sets the PID inputs, outputs and setpoint.
	* 	\param int : PID number
	*	\param double * : input
	*	\param double * : output
	* 	\param double * : setpoint
	*/
	void setPID(int, double *, double *, double *);

	private :

double *input[3];
double *output[3];
double *setpoint[3];

double Ki[3];
double Kp[3];
double Kd[3];

PID *pid_array[3];
};



#endif
