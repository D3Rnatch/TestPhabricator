/*  /!\Arduino 
	CLOSED LOOP CONTROL MAIN PROGRAM : Eq. Autopilot
	For Hovercraft using 4 lateral motors
*/

#include <Arduino.h>

// Perso libraries
#include "network_manager.h"
#include "pid_manager.h"
#include "scheduler.h"

// TODO : implementation
// Done : 45 %
#include "motor_handler.h"
// Done : 80%
#include "acquisition_handler.h"

// Particular #Defines
// TODO : setups
void setup()
{
	// Init Network
	// Init motors
	// Init PIDs
	// Init ACQ system
	// Init Scheduler
	// ping end of setup
}

// TODO : levels of update (motors, ACQ, etc...)
void loop()
{
	// Up network
	// Up ACQ
	// If 1/5 : run PIDs
	// Up motors
	// Up scheduler
}