#ifndef TIMER_IS_DEFINED
#define TIMER_IS_DEFINED

#ifdef ARDUINO
#include <Arduino.h>
#endif

#ifdef __linux__
extern "C"
{
#include <sys/time.h>
#include <alib-c/alib_time.h>
}
#endif

#include <stdio.h>
#include <limits.h>
#include <time.h>

namespace alib
{
	class Timer
	{
	private:
		unsigned long start_time = 0;
		unsigned long run_time;
		bool rang = true;
		
	public:
		Timer(size_t r_time);

		/* Starts/resets the timer. */	
		void begin();
		/* Checks to see if the timer has rung. */
		bool check();

		/* Setters */
		/* Sets the run time (in milliseconds) for the timer. */
		void setRunTime(size_t r_time);
		/***********/
	};
}
#endif
