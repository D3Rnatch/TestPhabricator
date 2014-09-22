/**
*	PID TUNING AND USE LIBRARY
*  This class uses the PID arduino library. It creates an object PID.
*/

#include <PID_v1.h>

#include "pid_parameters.h"

#ifndef PID_MANAGER_H
#define PID_MANAGER_H

// Headers...


// Inits...


// TODO : set up automatic tuning for PID parameters.
class PID_manager{
public :
	PID_manager()
	{
		// this function sets to default mode all PIDs and theirs parameters...
		this.pid_param[0] = pid_parameters();
		// this.pid_param[0].set_agg_param(); // NOT YET DONE.
		this.pid_param[0].set_cons_param(1,0.01,0);
		this.pid_tab[0] = myPID(&this.pid_param[0].Input, &this.pid_param[0].Output, &this.pid_param[0].Setpoint, this.pid_param[0].consKp, this.pid_param[0].consKi, this.pid_param[0].consKd, DIRECT);
		
		this.pid_param[1] = pid_parameters();
		// this.pid_param[0].set_agg_param(); // NOT YET DONE.
		this.pid_param[1].set_cons_param(1,0.01,0);
		this.pid_tab[1] = myPID(&this.pid_param[1].Input, &this.pid_param[1].Output, &this.pid_param[1].Setpoint, this.pid_param[1].consKp, this.pid_param[1].consKi, this.pid_param[1].consKd, DIRECT);
		
		this.pid_param[2] = pid_parameters();
		// this.pid_param[0].set_agg_param(); // NOT YET DONE.
		this.pid_param[2].set_cons_param(1,0.01,0);
		this.pid_tab[2] = myPID(&this.pid_param[2].Input, &this.pid_param[2].Output, &this.pid_param[2].Setpoint, this.pid_param[2].consKp, this.pid_param[2].consKi, this.pid_param[2].consKd, DIRECT);
	}
	
	void run_the_magic()
	{
		for(int i=0;i<3;i++)
		{
			this.pid_tab[i].compute();
		}
	}
	
	double getOutput(int a)
	{
		return this.pid_param[a].Output;
	}

	void setInput(int a, double input)
	{
		this.pid_param[a].Input = input;
	}
	
	void setPIDMode(int a, int mode)
	{
		this.pid_tab[a].SetMode(mode);
	}
	
	int getPIDMode(int)
	{
		return 	this.pid_tab[a].GetMode(mode);
	}
	
	void setNewPID(int a, pid_parameters np)
	{
		this.pid_tab[a] = myPID(&np.Input, &np.Output, &np.Setpoint, np.consKp, np.consKi, np.consKd, DIRECT);
		this.pid_param[a] = np;
	}
	
	PID getPID(int a)
	{
		return pid_tab[a];
	}
	
	void setPID(int a,PID p, pid_parameters np)
	{
		this.pid_tab[a] = p;
		this.pid_param = np;
	}
	
	void setParameters(int a, pid_parameters np)
	{
		this.pid_param[a] = np;
		this.pid_tab[a] = myPID(&np.Input, &np.Output, &np.Setpoint, np.consKp, np.consKi, np.consKd, DIRECT);
	}
	
	pid_parameters getParameters(int a)
	{
		return this.pid_param[a];
	}
	
private :
PID pid_tab[3]; // 2 linear PID on accelerometers and 1 on gytroscopic
pid_parameters pid_param[3]; // corresponding parameters...
};



#endif
