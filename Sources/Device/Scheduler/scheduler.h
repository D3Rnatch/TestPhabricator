/**	ARDUINO !!!
*	Scheduler : 
*    This class handles the schedule watch on the robot.
* Copyleft PFE Hovercraft 1408 - 2014/2015
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Timer.h"

#define DEFAULT_TIME 13

// TODO : Mettre en place un temps statistique de tour de la boucle program : 
// On ne cherche pas un temps par boucle mais dans le global à atteindre le temps voulu.
class Scheduler
{
	public :
		/**	\brief Constructor : Scheduler
		*
		*/
		Scheduler();
		
		
		/**	\brief Constructor : Scheduler
		*	\param : long : wanted loop time
		*/
		Scheduler(long);
	
		/**	\brief Updates the schedule data.
		*		Determines also time of the next delay.
		*/
		void update_scheduler();
		
		/** \brief Determines Loop Time
		*	
		*/
		long getLoopTime();
		
		/** \brief Regulate Loop : sets delay to reach wanted time
		*
		*/
		void regulate_loop();
		
		/// ATTACH EVENTS ZONE
		// Following functions let the user call functions at certain loop time.
		
	private :
		long last_loop_time;
		long now;
		long desired_time;
};

#endif