/**
*	PID TUNING AND USE LIBRARY
*  This class uses the PID arduino library. It creates an object PID.
*/

#include <PID_v1.h>

#ifndef PID_MANAGER_H
#define PID_MANAGER_H

// Headers...


// Inits...


class PID_manager{
public :


private :
PID pid_tab[3]; // 2 linear PID on accelerometers and 1 on gytroscopic

};



#endif
