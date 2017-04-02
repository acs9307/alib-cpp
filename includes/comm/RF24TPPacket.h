#ifndef RF24TP_PACKET_IS_DEFINED
#define RF24TP_PACKET_IS_DEFINED

#ifdef Arduino_h
#include <SPI.h>
#endif

#include <inttypes.h>
#include <stdlib.h>
#include <RF24/RF24.h>

namespace alib
{
	#define RF24TP_PACKET_LEN 32
	#define RF24TP_PACKET_HEADER_LEN 4
	#define RF24TP_PACKET_DATA_FIELD_LEN  (RF24TP_PACKET_LEN - RF24TP_PACKET_HEADER_LEN)

	class RF24TPPacket
	{
	private:
		/* Raw packet is in the following format.
		*
		* uint8_t data_id - ID of the packet.
		* uint8_t sender_id - ID of the sender.
		* uint16_t data_left - The number of bytes remaining in the data portion of the packet.
		* uint8_t data[28] - Data portion of the packet. */
		uint8_t raw[RF24TP_PACKET_LEN] = { 0 };

		/* Pointer to the original data, and will be used for incrementation.
		 * Used for streamlining the use of data. */
		const uint8_t* data_iter = NULL;

	public:
		/* Constructors */
		/* Constructs an empty RF24TPPacket. */
		RF24TPPacket();
		/* Constructs an RF24TPPacket with only the sender ID set.
		*
		* Parameters:
		*		senderId: The ID of the sender. */
		RF24TPPacket(uint8_t senderId);
		/* Constructs an RF24TPPacket.
		*
		* Parameters:
		*		senderId: The ID of the sender.
		*		dataId: The ID of the set of data packets.
		*		bytesLeft: The number of bytes remaining from the current and incoming
		*			packets. */
		RF24TPPacket(uint8_t senderId, uint8_t dataId,
			uint16_t bytesLeft);
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
		RF24TPPacket(uint8_t senderId, uint8_t dataId,
			uint16_t bytesLeft, const void* dataIter);
		/****************/

		/* Prepares data by placing data found at the data iterator into the
		* buffer.
		*
		* Returns:
		*		NULL: No data available in data_iter.
		*		uint8_t*: Pointer to the raw buffer of the packet. */
		uint8_t* preparePacket();
		/* Increments the data iterator and decrements the bytes left before calling
		* 'preparePacket()'.
		*
		* Returns:
		*		NULL: No data available in data_iter.
		*		uint8_t*: Pointer to the raw buffer of the packet. */
		uint8_t* prepareNextPacket();

		/* Getters */
		/* Gets a pointer to the raw buffer of the packet.
		*
		* This is useful when a packet is to be written or received. */
		uint8_t* getRaw() const;

		/* Gets the data ID of the packet. */
		uint8_t getDataId() const;
		/* Gets the sender's ID of the packet. */
		uint8_t getSenderId() const;
		/* Returns the number of bytes remaining for the message including
		* the data in the current packet. */
		uint16_t getBytesLeft() const;
		/* Returns the data portion of the packet. */
		uint8_t* getData() const;
		/* Returns the data iterator for the packet. This is included so that users
		* may check if the packet already has an iterator. */
		const uint8_t* getDataIter() const;
		/***********/

		/* Setters */
		/* Copies up to 'RF24TP_PACKET_LEN' bytes of the data in 'raw_pack' into the
		* raw buffer of the packet.
		*
		* If 'len' is less than 'RF24TP_PACKET_LEN', then the end of the packet will be
		* be filled with nulls. */
		void setRaw(uint8_t* raw_pack, uint8_t len);

		/* Sets the ID of the data.  This is used to determine which set of packets
		* belong together. */
		void setDataId(uint8_t dataId);
		/* Sets the ID of the sender. */
		void setSenderId(uint8_t senderId);
		/* Sets the number of bytes remaining for the message. */
		void setBytesLeft(uint16_t bytesLeft);
		/* Sets the data iterator for the packet.  This is used in 'preparePacket()' to fill
		* the data portion of the packet.
		*
		* This WILL NOT be copied, ensure that the life of the data surpasses that
		* of the packet.*/
		void setDataIter(const void* dataIter, uint16_t dataLen);
		/***********/
	};
}

#endif
