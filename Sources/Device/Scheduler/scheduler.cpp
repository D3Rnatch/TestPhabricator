
#include "scheduler.h"


		/**	\brief Constructor : Scheduler
		*
		*/
		Scheduler::Scheduler()
		{
			this->desired_time = DEFAULT_TIME;
		}
		
		
		/**	\brief Constructor : Scheduler
		*	\param : long : wanted loop time
		*/
		Scheduler::Scheduler(long t)
		{
			this->desired_time = t;
		}
	
		/**	\brief Updates the schedule data.
		*		Determines also time of the next delay.
		*/
		void Scheduler::update_scheduler()
		{
			this->now = millis()
		}
		
		/** \brief Determines Loop Time
		*	
		*/
		long Scheduler::getLoopTime()
		{
			return (this->now - this->last_loop_time);
		}
		
		/** \brief Regulate Loop : sets delay to reach wanted time
		*
		*/
		void Scheduler::regulate_loop()
		{
			long e = this->getLoopTime();
			e = e - this->desired_time;
			if(e<0)
				e = 0;
			
			delay(e);
		}