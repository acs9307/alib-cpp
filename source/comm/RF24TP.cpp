#ifdef ARDUINO
#include "includes/comm/RF24TP.h"
#else
#include "../includes/comm/RF24TP.h"
#endif

namespace alib
{
	/* Constructors */
	RF24TP::RF24TP(RF24& t) :transceiver(t) { t.setPayloadSize(RF24TP_PACKET_LEN); }
	RF24TP::RF24TP(RF24& t, bool shouldListen) : transceiver(t), listen(shouldListen) 
	{
		t.setPayloadSize(RF24TP_PACKET_LEN);
	}
	/****************/

	/* Transmission */
	void RF24TP::send(const void* d, uint16_t len, uint8_t senderId, uint8_t dataId)
	{
		if (len == 0)
			len = strlen((char*)d);

		RF24TPPacket packet = RF24TPPacket(senderId, dataId, (len)?len:strlen((char*)d), d);
		uint8_t* raw_data = packet.getRaw();

		transceiver.stopListening();
		do
		{
			transceiver.write(raw_data, RF24TP_PACKET_LEN);
		} while ((raw_data = packet.prepareNextPacket()));
		if(listen)
			transceiver.startListening();
	}
	void RF24TP::send(const void* data, uint16_t len, uint8_t senderId)
	{
#ifdef __linux__
		srandom(time(NULL));
		uint8_t ran = random();
		send(data, len, senderId, ran);
#else
		send(data, len, senderId, random(0, 255));
#endif
	}
	alib_error RF24TP::recv(void** out_buff, uint16_t* out_buff_len, size_t timeout)
	{
		if(!out_buff || !out_buff_len)return(ALIB_BAD_ARG);
		
		Timer t = Timer(timeout);
		alib_error rval = ALIB_OK;
		int16_t data_id = -1, sender_id = -1;
		uint16_t size_left = 0;
		uint8_t read_count = RF24TP_PACKET_DATA_FIELD_LEN;
		BinaryBuffer* buffer = newBinaryBuffer();
		if (!buffer)
			return(ALIB_MEM_ERR);

		RF24TPPacket packet = RF24TPPacket();
		
		/* Continue while there is data left from a client. */
		while(sender_id < 0 || size_left > 0)
		{
			/* Wait for data to become available. */
			t.begin();
			while(!transceiver.available() && !t.check()){delay(1);}
			if(t.check())
			{
				rval = ALIB_TIMEOUT;
				goto f_return;
			}
				
			/* Continue reading data until there is no more data left. */
			while(transceiver.available())
			{
				transceiver.read(packet.getRaw(), RF24TP_PACKET_LEN);
				
				/* If this is the first packet received, then we need to 
				 * set the id we will be using and the length of the total 
				 * data for error checking. */
				if(sender_id == -1)
				{
					data_id = packet.getDataId();
					sender_id = packet.getSenderId();
					size_left = packet.getBytesLeft();
				}
				/* If the id doesn't match up or the data length doesn't match, then we 
				 * have an error. */
				else if (sender_id != packet.getSenderId() || 
					size_left != packet.getBytesLeft())
				{
					rval = ALIB_TCP_SEND_ERR;
					goto f_return;
				}
				
				/* Check to see how much data we need to write to the buffer. */
				if(packet.getBytesLeft() < read_count)
					read_count = packet.getBytesLeft();
				BinaryBuffer_append(buffer, packet.getData(), read_count);
				
				/* Decrement the size left.  If there is nothing left, then
				 * we simply break. */
				size_left -= read_count;
				if(size_left == 0)
					break;
			}
		}
			
		/* Update the parameter pointers. */
		*out_buff_len = BinaryBuffer_get_length(buffer);
		*out_buff = BinaryBuffer_extract_buffer(buffer);
			
	f_return:
		delBinaryBuffer(&buffer);
		
		return(rval);
	}
		/****************/

	/* Getters */
	RF24& RF24TP::getTransceiver()const{return(transceiver);}
	/***********/
}
