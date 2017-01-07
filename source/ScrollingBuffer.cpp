#include "includes/ScrollingBuffer.h"

namespace alib
{
	/* Private Functions */
	/* Moves 'buffBegin' to the beginning of the buffer with an offset of
	 * ('buffBegin' - buffer end). */
	void ScrollingBuffer::wrapBuffBegin()
	{
		while (buffBegin >= buff + buffSize)
			buffBegin = buff + (buffBegin - (buff + buffSize));
	}
	/* Gets the pointer of the given index. */
	uint8_t* ScrollingBuffer::getPtr(size_t index) const
	{
		if (index >= buffSize)
			return(NULL);

		size_t tailLen = (buff + buffSize) - buffBegin;
		if (tailLen > index)
			return(buffBegin + index);
		else
			return(buff + (index - tailLen));
	}

	int ScrollingBuffer::getIndex(uint8_t* ptr)
	{
		if (!ptr)
			return(ALIB_BAD_ARG);
		if (ptr > buffBegin)
			return(ptr - buffBegin);

		int index = (buff + buffSize) - buffBegin;
		index += ptr - buff;
		return(index);
	}
	/*********************/

	/* Public Functions */
		/* Constructors */
	void ScrollingBuffer::init()
	{
		init(SB_DEFAULT_BUFF_SIZE);
	}
	void ScrollingBuffer::init(size_t bufferSize)
	{
		if (!bufferSize)
			buff = NULL;
		else
			buff = (uint8_t*)malloc(bufferSize);
		buffBegin = buff;
		buffSize = bufferSize;
		buffCount = 0;
	}

	ScrollingBuffer::ScrollingBuffer() :buffCount(0), buff(NULL), buffBegin(NULL), buffSize(0) {}
	ScrollingBuffer::ScrollingBuffer(size_t bufferSize) :buffSize(bufferSize), buffCount(0)
	{
		if (bufferSize)
			buff = (uint8_t*)malloc(bufferSize);
		else
			buff = NULL;
		buffBegin = buff;
	}

	ScrollingBuffer* ScrollingBuffer::NEW()
	{
		return(NEW(SB_DEFAULT_BUFF_SIZE));
	}
	ScrollingBuffer* ScrollingBuffer::NEW(size_t bufferSize)
	{
		ScrollingBuffer* sb = (ScrollingBuffer*)malloc(sizeof(ScrollingBuffer));
		if (!sb)return(NULL);

		sb->init(bufferSize);

		return(sb);
	}
	/****************/

	/* Adds a new value to the buffer, but will not remove any old values. */
	int ScrollingBuffer::append(uint8_t b)
	{
		if (buffCount == buffSize)
			return(ALIB_INTERNAL_MAX_REACHED);

		pushOn(b);
		return(1);
	}
	/* Appends an array to the buffer, but will not remove any old values. */
	int ScrollingBuffer::append(uint8_t* data, size_t dataLen)
	{
		if (!data)
			return(ALIB_BAD_ARG);
		else if (buffSize - buffCount < dataLen)
			dataLen = buffSize - buffCount;

		if(dataLen)
			pushOn(data, dataLen);
		return((int)dataLen);
	}
	/* Adds a new value to the buffer.  If the buffer is already full, then
	* the oldest data will be written over. */
	void ScrollingBuffer::pushOn(uint8_t b)
	{
		if (buffCount < buffSize)
		{
			uint8_t* ptr = getPtr(buffCount);
			++buffCount;

			*ptr = b;
		}
		else
		{
			*buffBegin = b;
			++buffBegin;
			wrapBuffBegin();
		}
	}
	/* Similar to 'pushOn(uint8_t)', except that an array is pushed on instead of a single byte.
	* If the buffer must wrap over old data, old data will be overwritten without warning. */
	void ScrollingBuffer::pushOn(uint8_t* data, size_t dataLen)
	{
		/* If the length of the given data is too long, then copy over the entire
		* buffer. */
		if (dataLen > buffSize)
		{
			data += dataLen - buffSize;
			dataLen = buffSize;
			buffBegin = buff;
			memcpy(buff, data, dataLen);
			buffCount = dataLen;
		}
		/* If there is nothing in the buffer, copy to the beginning of the internal buffer. */
		else if (!buffCount)
		{
			memcpy(buff, data, dataLen);
			buffCount = dataLen;
			buffBegin = buff;
		}
		/* If there is something in the buffer then push onto the backend of it. */
		else
		{
			/* Find the virtual end of the buffer. */
			uint8_t* vEnd = getPtr(buffCount - 1) + 1;

			/* Find the end of the internal buffer. */
			uint8_t* buffEnd = buff + buffSize;
			if (vEnd == buffEnd)
				vEnd = buff;

			/* Copy to the end of the internal buffer. */
			size_t copyCount = buffEnd - vEnd;
			if (copyCount > dataLen)
				copyCount = dataLen;
			memcpy(vEnd, data, copyCount);
			copyCount = dataLen - copyCount;

			/* If there is remaining data to copy, copy to the beginning of the internal buffer. */
			if (copyCount)
			{
				data += copyCount;
				vEnd = buff;
				memcpy(vEnd, data, copyCount);
			}

			/* Caculate the new count. */
			buffCount += dataLen;
			if (buffCount > buffSize)
			{
				buffBegin += buffCount - buffSize;
				buffCount = buffSize;
				wrapBuffBegin();
			}
		}
	}

	#if 0
	void ScrollingBuffer::sift()
	{
		if (buffBegin == buff)
			return;

		if (buffBegin + buffCount < buff + buffSize)
			memcpy(buff, buffBegin, buffCount);
		else
		{
			uint8_t buff1[SB_SIFT_BUFF_SIZE], buff2[SB_SIFT_BUFF_SIZE];
			uint8_t* fromBuff = buff1, *toBuff = buff2;
			size_t fromCount = 0, toCount = 0;
			uint8_t* vEnd = getPtr(buffCount - 1), *buffEnd = buff + buffSize;
			uint8_t* from = buffBegin, *to = buff, *cpyBuff;

			to = buff;
			from = buffBegin;
			do
			{
				/* Fill the from buffer if it is empty. */
				if (!fromCount)
				{
					fromCount = (sizeof(buff1) < buffEnd - from) ? sizeof(buff1) : buffEnd - from;
					memcpy(fromBuff, from, fromCount);
				}

				/* Fill the to buffer. */
				if (to < vEnd)
				{
					toCount = (sizeof(buff1) < vEnd - to) ? sizeof(buff1) : vEnd - to;
					memcpy(toBuff, to, toCount);
				}

				memcpy(to, fromBuff, fromCount);

				to += fromCount;
				fromBuff = toBuff;
				fromCount = toCount;
				toBuff = (toBuff == buff1) ? buff2 : buff1;
				toCount = 0;
			} while (to < buff + buffCount);
		}

		buffBegin = buff;
	}
	#endif
		/* Getters */
	/* Pulls the first byte out of the scrolling buffer.
	* Data pulled off is no longer available in the buffer. */
	int ScrollingBuffer::pullOff()
	{
		if (!buffCount)
			return(ALIB_BAD_INDEX);

		uint8_t rval = *buffBegin;
		--buffCount;
		++buffBegin;
		if (buffBegin >= buff + buffSize)
			buffBegin = buff;

		return(rval);
	}
	/* Pulls an array of bytes out from the beginning the scrolling buffer.
	* The returned value is dynamically allocated and MUST BE FREED BY THE CALLER.
	*
	* Data pulled off is no longer available in the buffer. */
	uint8_t* ScrollingBuffer::pullOff(size_t* count)
	{
		if (!count || !*count)return(NULL);
		if (*count > buffCount)
			*count = buffCount;

		/* Calculate how many bytes can be copied from the end of the buffer. */
		size_t copyCount = (buff + buffSize) - buffBegin;
		if (copyCount > *count)
			copyCount = *count;

		uint8_t* rval = (uint8_t*)malloc(*count);
		if (!rval)
		{
			*count = 0;
			goto f_return;
		}

		memcpy(rval, buffBegin, copyCount);
		buffBegin += copyCount;
		wrapBuffBegin();
		buffCount -= copyCount;

		if (copyCount < *count)
		{
			/* Use 'count' to temporarily store the remaining bytes to copy. */
			*count = *count - copyCount;
			memcpy(rval + copyCount, buffBegin, *count);
			buffCount -= *count;
			buffBegin += *count;

			/* Ensure we fix 'count' to be the total number of bytes that were copied. */
			*count += copyCount;
		}

	f_return:
		return(rval);
	}

	void ScrollingBuffer::clear()
	{
		buffBegin = buff;
		buffCount = 0;
	}

	/* Gets the byte at the given index of the scrolling buffer.
	* This is the index relative to the beginning of the buffer, not the
	* beginning of the internal buffer. */
	int16_t ScrollingBuffer::get(size_t index) const
	{
		if (index >= buffCount)
			return(ALIB_BAD_INDEX);

		uint8_t* ptr = getPtr(index);
		if (!ptr)
			return(ALIB_BAD_INDEX);
		else
			return(*ptr);
	}

	size_t ScrollingBuffer::count()const { return(buffCount); }
	size_t ScrollingBuffer::size()const { return(buffSize); }
		/***********/

		/* Setters */
	void ScrollingBuffer::setSize(size_t size)
	{
		buffCount = 0;
		buffSize = size;
		if (!buff && size)
			buff = (uint8_t*)malloc(size);
		else if (buffSize > 0 && buff)
			buff = (uint8_t*)realloc(buff, buffSize);
		else
		{
			if(buff)
				free(buff);
			buff = NULL;
		}
		buffBegin = buff;
	}
		/***********/

		/* Operator Overloads */
	int16_t ScrollingBuffer::operator[](size_t index)
	{
		return(get(index));
	}
		/**********************/

		/* Destructors */
	ScrollingBuffer::~ScrollingBuffer()
	{
		if (buff)
			free(buff);
	}

	void ScrollingBuffer::FREE(ScrollingBuffer* sb)
	{
		if (!sb)return;

		sb->~ScrollingBuffer();
		free(sb);
	}
	void ScrollingBuffer::DEL(ScrollingBuffer** sb)
	{
		if (!sb)return;

		FREE(*sb);
		*sb = NULL;
	}
		/***************/

	/********************/
}
