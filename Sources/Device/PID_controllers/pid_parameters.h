#ifndef PID_PARAMETERS_H
#define PID_PARAMETERS_H

class pid_parameters
{

public :
pid_parameters()
{

}

void set_agg_param(double kp, double ki, double kd)
{

}

void set_cons_param(double kp, double ki, double kd)
{

}

double Setpoint, Input, Output;
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;
};


#endif
