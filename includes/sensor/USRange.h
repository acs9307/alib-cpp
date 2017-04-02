#ifndef USRANGE_IS_DEFINED
#define USRANGE_IS_DEFINED

#ifdef ARDUINO
#include <Arduino.h>
#include <alib-c/includes/alib_error.h>
#else
#include <alib-c/alib_error.h>
#endif

#include <limits.h>

namespace alib
{
	#define US_CM_PER_MICROS 0.03432
	#define US_MICROS_PER_CM (1 / US_CM_PER_MICROS)

	/* One library I found used 500 cm as max distance. */
	#define US_MAX_RANGE_CM 500
		
	/* Maximum ammount of time to wait for a ping to be received. 
	 * Multiply by two as we have to calculate time to the target plus time 
	 * back from the target. */
	#define US_MAX_PING_TIME ((US_MICROS_PER_CM * US_MAX_RANGE_CM) * 2)	//Micros

	/* Waiting for the sensor to respond after initialization can take some time. */
	#define US_MAX_SENSOR_RESPONSE_TIME 35	//Millis
	
	class USRange
	{
	private:
		uint8_t trig;
		uint8_t echo;

		uint16_t maxRange; //micros
	public:

		/* Constructors */
		void init();

		USRange(uint8_t trigPin, uint8_t echoPin);
		USRange(uint8_t trigPin, uint8_t echoPin, size_t maxRange_cm);
		/****************/

		/* Functions */
		/* Initializes the sensor for pinging. */
		alib_error startPing();

		/* Returns the number of microseconds to the object. */
		int16_t getPingTime();

		/* Returns the distance from the object in front of the ultra sonic sensor in centimeters. 
		 * Returns -1 if the object is farther than the maximum distance from the sensor. */
		int16_t getDistanceCM();
		/*************/
	};
}
#endif

