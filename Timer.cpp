#include "Timer.h"

namespace alib
{
	Timer::Timer(size_t r_time):run_time(r_time){}

	/* Starts/resets the timer. */	
	void Timer::begin()
	{
#ifdef __linux__
		struct timeval now;
		gettimeofday(&now, NULL);
		start_time = SECONDS_TO_MILLIS(now.tv_sec) + 
			MICROS_TO_MILLIS(now.tv_usec);
#else
		start_time = millis();
#endif
		rang = false;
	}
	/* Checks to see if the timer has rung. */
	bool Timer::check()
	{
		if(rang)return(rang);
	
#ifdef __linux__	
		struct timeval now;
		gettimeofday(&now, NULL);
		unsigned long long curMillis = SECONDS_TO_MILLIS(now.tv_sec) + 
			MICROS_TO_MILLIS(now.tv_usec);
		rang = (curMillis - start_time) >= run_time;
#else
		rang = (millis() - start_time) >= run_time;
#endif
		return(rang);
	}

	/* Setters */
	/* Sets the run time (in milliseconds) for the timer. */
	void Timer::setRunTime(size_t r_time){run_time = r_time;}
	/***********/
}
