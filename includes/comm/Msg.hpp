#ifndef ALIB_CPP_COMM_MSG_IS_DEFINED
#define ALIB_CPP_COMM_MSG_IS_DEFINED

extern "C"
{
#include <stdint.h>
#include <stdio.h>
}

/* NOTE:
 * THIS IS AN HPP FILE BECAUSE DEFINING VIRTUAL FUNCTIONS IN DIFFERENT FILE CAUSES LINKER ERRORS WITH TEMPLATED CLASSES.... */

namespace alib
{
	namespace comm
	{
		/* Interface object for Msg objects.  This is needed because a template is required to specify a specific
		* Msg object.  If we have an interface for the base class, then, even though templates are used, we can generalize
		* objects.  Use this to generalize messages. */
		class IMsg
		{
		public:
			/* Due to MsgWrapper design, we can't utilize a constructor for 
			 * this class, so any constructors building from binary arrays
			 * should simply call this function. */
			virtual void copyBuff(const uint8_t* data) = 0;
			virtual void copyBuff(const uint8_t* data, size_t dataLen) = 0;
		
			/* Getters */
			/* Returns a pointer to the raw binary data of the message. */
			virtual const uint8_t* getBytes() const = 0;
			/* Returns the size of the buffer in bytes. */
			virtual size_t getBuffSize() const = 0;
			/***********/

#ifdef ARDUINO
			/* Prints the data to a given stream in human readable format. */
			virtual void print(Stream& stream) = 0;
#endif

			/* Ensures that a Msg is actually valid.
			* This is useful if the object is constructed from binary. */
			virtual bool isValid()const = 0;
			/*********************/
		};

		/* Object interface used for communication, specifically designed to be used with I2C, but can be
		* used with any communication protocol/medium.
		*
		* The object is a wrapper for a simple binary buffer, therefore all data and members are stored
		* inside this buffer.  All derived objects MUST implement getter and setter functions
		* for each member related to the message and then store those values directly into the buffer.
		* So long as the derived object's buffer size is the same or less than the original's object's
		* buffer size, this can be cast to any derived Msg object.
		*
		* When implemented correctly, derived classes should never have to worry about the base class's
		* placement and implementation, only on how the related portion of the message is implemented.
		*
		* Template Parameters:
		*		UTILIZED_SIZE:	The number of bytes within the buffer that are actually used by the object.
		*							This includes 
		*		BUFF_SIZE:		The total number of bytes to allocate in the buffer for the message.
		*						 This should be specified if you need a common message length so the
		*						 requester knows how many bytes to except.
		*/
		template<size_t UTILIZED_SIZE, size_t BUFF_SIZE = UTILIZED_SIZE>
		class Msg : public IMsg
		{
		private:
			/* Ensure we are given valid values in the template. */
			static_assert(UTILIZED_SIZE <= BUFF_SIZE, "UTILIZED_SIZE cannot exceed BUFF_SIZE!");

			/* The entire packet that will be transmitted, all data is stored here and only
			* linked to from getter and setter functions.
			* Does this need to be volatile? */
			uint8_t _raw[BUFF_SIZE];

		protected:
			/* I'm calling these auto virtual functions because it automatically handles changes for derived classes without
			 * explicitly being defined.  They should never be overridden and therefore are not an actual virtual 
			 * type function. Auto Virtual Functions due to template class inheritance. */
			/* Auto Virtual Functions */
				/* Getters */
			/* NOTE: We do not make '_getDerivedBuff()' virtual here as we should never need to modify the returned value.
			* This works, even for base classes, as when the base class calls the function, it should call its
			* base implementation '_getDerivedBuff()' which should not have the most derived class's DERIVED_SIZE. 
			* I.E. 
			*	template <DERIVED_SIZE>
			*	class Base : Msg<DERIVED_SIZE + 1>;
			*
			*	class Derived : Base<1>;
			*
			**/
			/* Getter for getting the related buffer for the message. */
			uint8_t* _getDerivedBuff() { return((uint8_t*)_raw + UTILIZED_SIZE); }
			const uint8_t* _getDerivedBuff() const { return((uint8_t*)_raw + UTILIZED_SIZE); }
				/***********/
			/**************************/

				/* Getters */
			/* Gets the value at the given offset to the specified value.
			*
			* Parameters:
			*		offset: The offset, in bytes, at which to retrieve the value in the message buffer. */
			template<typename T>
			T _getDerivedOffset(size_t offset) const { return(*((T*)(_raw + offset))); }
			/* Gets the value at the given offset to the specified value.
			 * Use the above functions unless you have a shitty compiler like the Arduino one which does not allow 
			 * 		recursive template calls...
			 *
			 * Parameters:
			 *		offset: The offset, in bytes, at which to retrieve the value in the message buffer. */
			const void* _getDerivedOffset(size_t offset)
			{
				return(_raw + offset);
			}

			/* Returns utilized size, but we want to use this for inheritance reasons. */
			virtual size_t _getDerivedSize()const { return(UTILIZED_SIZE); }
				/***********/

				/* Setters */
			/* Sets the value at the given offset to the specified value.
			*
			* Parameters:
			*		offset: The offset, in bytes, at which to place the value in the message buffer.
			*		value: The value to set at the given offset. */
			template<typename T>
			void _setDerivedOffset(size_t offset, T value) 
			{ 
				*((T*)(_raw + offset)) = value;
			}
			/* Sets the value at the given offset to the specified value.
			 * Use the above functions unless you have a shitty compiler like the Arduino one which does not allow 
			 *  		recursive template calls...
			 *
			 * Parameters:
			 *		offset: The offset, in bytes, at which to place the value in the message buffer.
			 *		value: Pointer to the value to set at the given offset. 
			 * 		valueSize: The size of the value in bytes. */
			void _setDerivedOffset(size_t offset, void* value, size_t valueSize) 
			{ 
				uint8_t* valIt = (uint8_t*)value;
				uint8_t* it = _raw + offset;
				for(; valueSize; --valueSize, ++it, ++valIt)
					*it = *valIt;
			}
				/***********/
			/*********************/
		public:		
			/* Static Functions */
			/* Returns the length of the message in bytes. 
			 * Same as 'getBuffSize()' but nice to have when you need a static value. */
			static constexpr inline size_t getBuffSizeStatic(){ return(BUFF_SIZE); }
			/* Number of bytes actually used by the object.  This may be less than the total
			 * size as some messages may want a common buffer size for expansion. */
			static size_t getUtilizedSize(){ return(UTILIZED_SIZE); }
			/* Returns the number of bytes that are unused. */
			static size_t getRemainingSize() { return(BUFF_SIZE - UTILIZED_SIZE); }
			/********************/

			/* Overridden Functions */
			/* NOTE: All virtual functions must be defined here and not in the source file, weird
			 * linker error is thrown otherwise. */
			/* Virtual Functions */
			void copyBuff(const uint8_t* data) { copyBuff(data, sizeof(_raw)); }
			void copyBuff(const uint8_t* data, size_t dataLen)
			{
				if (dataLen > sizeof(_raw))
					dataLen = sizeof(_raw);
				memcpy(_raw, data, dataLen);
			}
			
				/* Getters */
			/* Returns a pointer to the raw binary data of the message. */
			const uint8_t* getBytes() const { return(_raw); }
			/* Returns the size of the buffer in bytes. */
			size_t getBuffSize()const{return(getBuffSizeStatic());}
				/***********/
			/************************/
		};
	}
}

#endif
