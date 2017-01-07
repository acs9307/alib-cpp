#ifndef SCROLLING_BUFFER_IS_DEFINED
#define SCROLLING_BUFFER_IS_DEFINED

#ifdef ARDUINO
#include <alib-c/includes/alib_error.h>
#else
#include <alib-c/alib_error.h>
#endif

extern "C"
{
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
}

namespace alib
{
	#ifndef SB_SIFT_BUFF_SIZE
	#define SB_SIFT_BUFF_SIZE 32
	#endif

	#ifndef SB_DEFAULT_BUFF_SIZE
	#define SB_DEFAULT_BUFF_SIZE 32
	#endif
	
	/* Object used to efficiently store data into a single buffer that may need to be wrapped, 
	 * meaning that if a buffer of 32 bytes long is full, then 5 more bytes is appended to it, then 
	 * the new data would be placed at the beginning of the internal buffer.  Use ability should be similar
	 * to any other buffer/array object. */
	class ScrollingBuffer
	{
	private:
		uint8_t* buff;
		uint8_t* buffBegin;
		size_t buffSize;
		size_t buffCount;

		/* Moves 'buffBegin' to the beginning of the buffer with an offset of 
		 * ('buffBegin' - buffer end). */
		void wrapBuffBegin();
		/* Gets the pointer of the given index. */
		uint8_t* getPtr(size_t index) const;

		int getIndex(uint8_t* ptr);

	public:
		/* Constructors */
		void init();
		void init(size_t bufferSize);

		ScrollingBuffer();
		ScrollingBuffer(size_t bufferSize);

		static ScrollingBuffer* NEW();
		static ScrollingBuffer* NEW(size_t bufferSize);
		/****************/

		/* Adds a new value to the buffer, but will not remove any old values. */
		int append(uint8_t b);
		/* Appends an array to the buffer, but will not remove any old values. */
		int append(uint8_t* data, size_t dataLen);
		/* Adds a new value to the buffer.  If the buffer is already full, then 
		 * the oldest data will be written over. */
		void pushOn(uint8_t b);
		/* Similar to 'pushOn(uint8_t)', except that an array is pushed on instead of a single byte. 
		 * If the buffer must wrap over old data, old data will be overwritten without warning. */
		void pushOn(uint8_t* data, size_t dataLen);

	#if 0
		void sift();
	#endif
		/* Getters */
		/* Pulls the first byte out of the scrolling buffer. 
		 * Data pulled off is no longer available in the buffer. */
		int pullOff();
		/* Pulls an array of bytes out from the beginning the scrolling buffer. 
		 * The returned value is dynamically allocated and MUST BE FREED BY THE CALLER. 
		 * 
		 * Data pulled off is no longer available in the buffer. */
		uint8_t* pullOff(size_t* count);

		void clear();

		/* Gets the byte at the given index of the scrolling buffer. 
		 * This is the index relative to the beginning of the buffer, not the 
		 * beginning of the internal buffer. */
		int16_t get(size_t index) const;

		size_t count()const;
		size_t size()const;
		/***********/

		/* Setters */
		void setSize(size_t size);
		/***********/

		/* Operator Overloads */
		int16_t operator[](size_t index);
		/**********************/

		/* Destructors */
		~ScrollingBuffer();

		static void FREE(ScrollingBuffer* sb);
		static void DEL(ScrollingBuffer** sb);
		/***************/
	};
}


#endif
