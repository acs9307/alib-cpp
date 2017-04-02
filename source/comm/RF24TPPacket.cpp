#include "includes/comm/RF24TPPacket.h"

namespace alib
{
	/* Constructors */
	/* Constructs an empty RF24TPPacket. */
	RF24TPPacket::RF24TPPacket() {}
	/* Constructs an RF24TPPacket with only the sender ID set. 
	 * 
	 * Parameters:
	 *		senderId: The ID of the sender. */
	RF24TPPacket::RF24TPPacket(uint8_t senderId) { setSenderId(senderId); }
	/* Constructs an RF24TPPacket. 
	 * 
	 * Parameters:
	 *		senderId: The ID of the sender. 
	 *		dataId: The ID of the set of data packets.
	 *		bytesLeft: The number of bytes remaining from the current and incoming 
	 *			packets. */
	RF24TPPacket::RF24TPPacket(uint8_t senderId, uint8_t dataId,
		uint16_t bytesLeft) 
	{ 
		setSenderId(senderId); 
		setDataId(dataId);
		setBytesLeft(bytesLeft);
	}
	/* Constructs an RF24TPPacket.
	 *
	 * The data found at 'dataIter' will be copied into the internal buffer. 
	 *
	 * Parameters:
	 *		senderId: The ID of the sender.
	 *		dataId: The ID of the set of data packets.
	 *		bytesLeft: The number of bytes remaining from the current and incoming
	 *			packets. 
	 *		dataIter: A pointer to the beginning of the data to be used for preparing
	 *			the data. */
	RF24TPPacket::RF24TPPacket(uint8_t senderId, uint8_t dataId,
		uint16_t bytesLeft, const void* dataIter):data_iter((uint8_t*)dataIter)
	{
		setSenderId(senderId);
		setDataId(dataId);
		setBytesLeft(bytesLeft);
		preparePacket();
	}
	/****************/

	/* Prepares data by placing data found at the data iterator into the 
	 * buffer. 
	 *
	 * Returns:
	 *		NULL: No data available in data_iter. 
	 *		uint8_t*: Pointer to the raw buffer of the packet. */
	uint8_t* RF24TPPacket::preparePacket()
	{
		if (!data_iter || !getBytesLeft())return(NULL);

		if (getBytesLeft() < RF24TP_PACKET_DATA_FIELD_LEN)
		{
			memset(raw + getBytesLeft(), 0, RF24TP_PACKET_DATA_FIELD_LEN - getBytesLeft());
			memcpy(getData(), data_iter, getBytesLeft());
		}
		else
			memcpy(getData(), data_iter, RF24TP_PACKET_DATA_FIELD_LEN);

		return(raw);
	}
	/* Increments the data iterator and decrements the bytes left before calling
	 * 'preparePacket()'. 
	 *
	 * Returns:
	 *		NULL: No data available in data_iter. 
	 *		uint8_t*: Pointer to the raw buffer of the packet. */
	uint8_t* RF24TPPacket::prepareNextPacket()
	{
		if (!data_iter || getBytesLeft() == 0)return(NULL);

		if (getBytesLeft() <= RF24TP_PACKET_DATA_FIELD_LEN)
		{
			setBytesLeft(0);
			data_iter = NULL;
			return(NULL);
		}
		else
		{
			setBytesLeft(getBytesLeft() - RF24TP_PACKET_DATA_FIELD_LEN);
			data_iter += RF24TP_PACKET_DATA_FIELD_LEN;
		}

		return(preparePacket());
	}

	/* Getters */
	/* Gets a pointer to the raw buffer of the packet. 
	 * 
	 * This is useful when a packet is to be written or received. */
	uint8_t* RF24TPPacket::getRaw() const { return((uint8_t*)raw); }

	/* Gets the data ID of the packet. */
	uint8_t RF24TPPacket::getDataId() const { return(raw[0]); }
	/* Gets the sender's ID of the packet. */
	uint8_t RF24TPPacket::getSenderId() const { return(raw[1]); }
	/* Returns the number of bytes remaining for the message including
	 * the data in the current packet. */
	uint16_t RF24TPPacket::getBytesLeft() const 
	{ 
		return(*((uint16_t*)(raw + 2)));
	}
	/* Returns the data portion of the packet. */
	uint8_t* RF24TPPacket::getData() const { return((uint8_t*)(raw + 4)); }
	/* Returns the data iterator for the packet. This is included so that users
	 * may check if the packet already has an iterator. */
	const uint8_t* RF24TPPacket::getDataIter() const { return(data_iter); }
	/***********/

	/* Setters */
	/* Copies up to 'RF24TP_PACKET_LEN' bytes of the data in 'raw_pack' into the 
	 * raw buffer of the packet. 
	 * 
	 * If 'len' is less than 'RF24TP_PACKET_LEN', then the end of the packet will be
	 * be filled with nulls. */
	void RF24TPPacket::setRaw(uint8_t* raw_pack, uint8_t len)
	{
		if (len < RF24TP_PACKET_LEN)
			memset(raw + len, 0, RF24TP_PACKET_LEN - len);
		else
			len = RF24TP_PACKET_LEN;

		memcpy(raw, raw_pack, len);
	}

	/* Sets the ID of the data.  This is used to determine which set of packets
	 * belong together. */
	void RF24TPPacket::setDataId(uint8_t dataId) { raw[0] = dataId; }
	/* Sets the ID of the sender. */
	void RF24TPPacket::setSenderId(uint8_t senderId) { raw[1] = senderId; }
	/* Sets the number of bytes remaining for the message. */
	void RF24TPPacket::setBytesLeft(uint16_t bytesLeft) 
	{ 
		*((uint16_t*)(raw + 2)) = bytesLeft;
	}

	/* Sets the data iterator for the packet.  This is used in 'preparePacket()' to fill
	 * the data portion of the packet. 
	 * 
	 * This WILL NOT be copied, ensure that the life of the data surpasses that
	 * of the packet.*/
	void RF24TPPacket::setDataIter(const void* dataIter, uint16_t dataLen) 
	{ 
		data_iter = (uint8_t*)dataIter; 
		setBytesLeft(dataLen);
	}
	/***********/
}