#include "includes/sensor/USRange.h"

namespace alib
{
	/* Constructors */
	void USRange::init()
	{
		pinMode(trig, OUTPUT);
		digitalWrite(trig, LOW);

		pinMode(echo, INPUT_PULLUP);
	}

	USRange::USRange(uint8_t trigPin, uint8_t echoPin) :trig(trigPin), echo(echoPin), maxRange(US_MAX_PING_TIME)
	{
		init();
	}
	USRange::USRange(uint8_t trigPin, uint8_t echoPin, size_t maxRange_cm) :
		trig(trigPin), echo(echoPin),
		maxRange((maxRange_cm * US_MICROS_PER_CM) * 2)
	{
		init();
	}
	/****************/

	/* Functions */
	/* Initializes the sensor for pinging. */
	alib_error USRange::startPing()
	{
		digitalWrite(trig, LOW);
		delayMicroseconds(4);
		digitalWrite(trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig, LOW);

		/* Wait for the sensor to respond. */
		unsigned long endTime = millis() + US_MAX_SENSOR_RESPONSE_TIME;
		if (endTime < millis())
			while (digitalRead(echo) != HIGH && millis() > endTime);
		while (digitalRead(echo) != HIGH && millis() < endTime);
		if (digitalRead(echo) == LOW)
			return(ALIB_DEVICE_NOT_FOUND);
	}

	/* Returns the number of microseconds to the object. */
	int16_t USRange::getPingTime()
	{
		unsigned long endTime, startTime;

		/* Prepares for pinging. */
		if((endTime = startPing()) < 0)
			return(endTime);

		/* Wait for the echo pin to go low or for the maximum range to be hit. */
		startTime = micros();
		endTime = startTime + maxRange;
		if (micros() > endTime)
		{
			while (digitalRead(echo) == HIGH && endTime < micros());
			if (digitalRead(echo) == LOW)
				return((micros() - startTime) / 2);
		}
		while (digitalRead(echo) == HIGH && endTime > micros());
		if (digitalRead(echo) == LOW)
			return((micros() - startTime) / 2);
		else
			return(-1);
	}

	/* Returns the distance from the object in front of the ultra sonic sensor in centimeters.
	* Returns -1 if the object is farther than the maximum distance from the sensor. */
	int16_t USRange::getDistanceCM()
	{
		int16_t time = getPingTime();
		if (time < 0)
			return(time);
		else
			return(time * US_CM_PER_MICROS);
	}
	/*************/
}