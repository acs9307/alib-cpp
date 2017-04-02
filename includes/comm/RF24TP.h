#ifndef DATA_PACKET_IS_DEFINED
#define DATA_PACKET_IS_DEFINED

#include <inttypes.h>

extern "C"
{
#ifdef ARDUINO
#include <alib-c/includes/BinaryBuffer.h>
#include <alib-c/includes/alib_error.h>
#else
#include <alib-c/BinaryBuffer.h>
#include <alib-c/alib_error.h>
#endif

#ifdef __linux__
#include <time.h>
#include <stdlib.h>
#endif
}

#include <RF24/nRF24L01.h>
#include <RF24/RF24.h>

#ifdef ARDUINO
#include <alib-cpp/includes/Timer.h>
#else
#include <alib-cpp/Timer.h>
#endif

#include "RF24TPPacket.h"

namespace alib
{
	class RF24TP
	{
	private:	
		RF24& transceiver;
		bool listen = false;
		
	public:

		/* Constructors */
		RF24TP(RF24& t);
		RF24TP(RF24& t, bool shouldListen);
		/****************/

		/* Transmission */
		void send(const void* d, uint16_t len, uint8_t senderId, uint8_t dataId);
		void send(const void* data, uint16_t len, uint8_t senderId);
		alib_error recv(void** out_buff, uint16_t* out_buff_len, size_t timeout);
			/****************/

		/* Getters */
		RF24& getTransceiver()const;
		bool willListen()const;
		/***********/

		/* Setters */
		void setListening(bool shouldListen);
		/***********/
	};
}

#endif
