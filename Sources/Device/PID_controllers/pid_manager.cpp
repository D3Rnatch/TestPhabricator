/**	This source implements pid_manager class
**
**
**/
	/**
	* \brief this function creates the array of PID and initialize them.
	* \param :
	*/
	PID_manager::PID_manager(double Kp, double Ki, double Kd)
	{
		this->Ki = Ki;
		this->Kd = Kd;
		this->Kp = Kp;
	}
	
	PID_manager::PID_manager()
	{
		this->Ki = KI;
		this->Kd = KD;
		this->Kp = KP;
	}
	
	/**
	*
	*
	*/
	void PID_manager::run_the_magic()
	{
		int i;
		for(i=0;i<3;i++)
			this->pid_array[i]->compute();
	}
	
	/**
	*
	*
	*/
	void PID_manager::compute_selective(int a)
	{
		if (a< 3 && a >= 0)
			this->pid_array[a]->compute();
	}

	/**
	*	\brief setPID : sets the PID inputs, outputs and setpoint.
	* 	\param int : PID number
	*	\param double * : input
	*	\param double * : output
	* 	\param double * : setpoint
	*/
	void PID_manager::setPID(int a, double *i, double *o, double *s)
	{    
		if(a< 3 && a >= 0)
		{
			this->pid_array[a] = new PID(i,o,s,this->Kp, this->Ki, this->Kd, DIRECT);
			this->pid_array[a]->SetMode(AUTOMATIC);
			this->pid_array[a]->setOutputLimits(-128,127);
		}
	}