#ifndef ALIB_COMM_MSG_IS_DEFINED
#define ALIB_COMM_MSG_IS_DEFINED

#ifdef ARDUINO
#include <Arduino.h>
#include <new.h>
#else
#include <iostream>
#endif

namespace alib
{
	namespace comm
	{
		/* Generic list of constructors all inheriting objects should implement. 
		 * Note that these are not required, but greatly simplify creation of most Msg objects. */
#define ALIB_COMM_MSG_INHERITED_CONSTRUCTORS(DERIVED_CLASS, BASE_CLASS) \
		DERIVED_CLASS(const Msg& msg, size_t buffSize = 0, bool referenceBuff = true) :BASE_CLASS(msg, buffSize, referenceBuff) {}

		/* Generic list of constructors all inheriting objects should implement. 
		 * Note that these are not required, but greatly simplify creation of most Msg objects.*/
#define ALIB_COMM_MSG_INHERITED_CONSTRUCTORS_STATIC_SIZE(DERIVED_CLASS, BASE_CLASS) \
		DERIVED_CLASS(const Msg& msg, bool referenceBuff = true) : BASE_CLASS(msg, referenceBuff) {}

		/* Set of functions that MUST BE IMPLEMENTED but should always be the same with each derivative. 
		 * Note that implementing this MUST HAVE permissions scoping re-declared as it defines functions of 
		 * multiple permission type. */
#define ALIB_COMM_MSG_INHERITED_FUNCTIONS(DERIVED_CLASS, BASE_CLASS, UTILIZED_SIZE)\
	protected:\
		virtual size_t _getBaseSize()const{return(BASE_CLASS::getUtilizedSize());}\
	public:\
		virtual size_t getUtilizedSize()const{return(DERIVED_CLASS::_getBaseSize() + UTILIZED_SIZE);}

		/* Abstract class that simply wraps a buffer for communication. */
		class Msg
		{
		private:
			void* _buff;
			size_t _buffSize = 0;
			bool _freeOnDestroy;

		protected:
			/* Functions */
				/* Constructors */
			/* Basically sets all values to empty, will not allocate memory.  This should be followed by a call to '_init()'. 
			 * This is really useful whenever inheriting classes want to provide an interface for converting a buffer into a Msg object.
			 * Originally it was desired to place such a function within Msg itself, however to do this would require Msg not be abstract... */
			Msg() :_buff(NULL), _freeOnDestroy(false) {}

			/* Constructs a Msg object and sets the internal buffer to point to the given one.  If 'buff' is NULL and 'buffSize' is not zero, then
			 * the buffer will be dynamically created and its lifecycle will be handled by the object. */
			Msg(void* buff, size_t buffSize = 0, bool own = false) { _init(buff, buffSize, own); }
				/****************/

				/* Virtual */
					/* Getters */
			virtual size_t _getBaseSize()const { return(0); }
					/***********/
				/***********/

			/* Initializes a Msg object with the given buffer.  Should be used after calling the default constructor. 
			*
			* Parameters:
			*		buff: The buff to either reference or copy.
			*		buffSize: The length of 'buff' or the number of bytes to allocate.
			*		own: If true, the Msg object will own the buffer and handle its lifecycle.
			*/
			void _init(void* buff, size_t buffSize = 0, bool own = false)
			{
				_buffSize = buffSize;

				if (!buff)
					own = true;
				_freeOnDestroy = own;

				if (buff)
					_buff = buff;
				else if (buffSize > 0)
					_buff = new uint8_t[buffSize];
				else
					_buff = NULL;
			}

			/* Clears the derived buff up to count 'byteCount'. 
			 * if 'byteCount' == 0, then the entire derived buff will be cleared. */
			void _clearDerivedBuff(size_t baseSize, size_t byteCount = 0)
			{
				if (!byteCount)
					byteCount = _buffSize - baseSize;

				void* derBuff = _getDerivedBuff(baseSize);
				memset(_getDerivedBuff(baseSize), 0, byteCount);
			}

				/* Getters */
			/* Gets the buffer for the derived class (this is based off of the logical offset). */
			void* _getDerivedBuff(size_t baseSize)
			{
				if (!_buff)return(NULL);

				return(((uint8_t*)_buff) + baseSize);
			}
			/* Gets the buffer for the derived class (this is based off of the logical offset). */
			const void* _getDerivedBuff(size_t baseSize)const
			{
				if (!_buff)return(NULL);

				return(((uint8_t*)_buff) + baseSize);
			}

			/* Gets the offset of the derived buff and returns the requested type. 
			 * 
			 * Parameters:
			 *		offset: The number of bytes to offset from the derived buffer.
			 *		baseSize: Utilized size of the base class. */
			template<typename T>
			T _getDerivedOffset(size_t offset, size_t baseSize)
			{
				if (!_buff)return(T());

				return(*((T*)((uint8_t*)_getDerivedBuff(baseSize) + offset)));
			}
			/* Gets the offset of the derived buff and returns the requested type.
			*
			* Parameters:
			*		offset: The number of bytes to offset from the derived buffer.
			*		baseSize: Utilized size of the base class. */
			template<typename T>
			const T _getDerivedOffset(size_t offset, size_t baseSize) const
			{
				if (!_buff)return(T());

				return(*((T*)((uint8_t*)_getDerivedBuff(baseSize) + offset)));
			}
				/***********/

				/* Setters */
			/* Sets the value at the provided offset. */
			template<typename T>
			void _setDerivedOffset(size_t offset, size_t baseSize, T val)
			{
				if (!_buff)return;

				*((T*)(((uint8_t*)_getDerivedBuff(baseSize)) + offset)) = val;
			}
				/***********/
			/*************/

		public:
			/* Constructors */
			/* Copy constructor.  
			 * This will either create a shallow copy (referencing the same buffer), or 
			 * create a deep copy (allocating a new buffer, and copying all of 'msg's buffer data into the new buffer).
			 * 
			 * Note: As with all Msg objects, using a referenced buffer means that the buffer must outlive the Msg object.
			 * 
			 * Parameters:
			 *		msg: The message to copy.
			 *		buffSize: The length of the buffer, in bytes. If 0, the buffer from 'msg' will be used.
			 *		referenceBuff: If true, the internal buffer of 'msg' will be referenced and this object
			 *						will not own it. */
			Msg(const Msg& msg, size_t buffSize = 0, bool referenceBuff = true) { _init(((referenceBuff) ? msg._buff : NULL), (buffSize) ? buffSize : msg._buffSize); }
			/****************/

			/* Functions */
				/* Virtual */
					/* Getters */
			/* Returns the number of bytes utilized by the message.  This differs from the actual buffer size.
			 * i.e. buffer size >= utilized size */
			virtual size_t getUtilizedSize()const { return(0); }
			/* Ensures that a Msg is actually valid.
			 * This is useful if the object is constructed from binary. */
			virtual bool isValid()const = 0;
					/***********/
				/***********/

			/* Copies a buffer into the given message. */
			void copy(const void* data, size_t dataLen = 0)
			{
				if (!data)return;
				if (dataLen == 0)
					dataLen = getBuffSize();

				memcpy(_buff, data, dataLen);
			}
			/* Copies the buffer of the given message. */
			void copy(const Msg& msg) { copy(msg._buff, msg._buffSize); }

			/* Attempts to take ownership of the internal buffer from the given Msg object.
			 * This will only work if both messages are referencing the same buffer and the given 'msg' is 
			 * the owner of the buffer.
			 *
			 * Returns:
			 *		true: Ownership was taken.
			 *		false: Ownership could not be taken, either due to the fact that 'msg' is not the buffer's owner
			 *			   or that 'msg' and this object are not pointing to the same buffer. */
			bool takeOwnership(Msg& msg)
			{
				if (msg._freeOnDestroy && msg._buff == _buff)
				{
					msg._freeOnDestroy = false;
					_freeOnDestroy = true;
					return(true);
				}
				else
					return(false);
			}

				/* Getters */
			/* Returns true if the object owns the buffer (will free on destruction). */
			bool buffIsOwned()const { return(_freeOnDestroy); }
			/* Returns the size of the buffer in bytes. 
			 * 0 is returned when the buffer size was not specified. */
			size_t getBuffSize()const { return(_buffSize); }
			/* Gets a pointer to the internal buffer. */
			const void* getBuff()const { return(_buff); }
			/* Same as 'getBuff()' but gets a writable buffer.  This should be used to write directly to the buffer.
			 * BE VERY CAREFUL WITH THIS, and DEFFINATELY NEVER FREE the returned buffer. */
			void* getWritableBuff()const{return(_buff);}
			/* Returns the number of bytes that are unutilized. */
			size_t getRemainingSize()const { return(getBuffSize() - getUtilizedSize()); }
				/***********/
			/*************/

			~Msg()
			{
				if (_freeOnDestroy && _buff)
					delete[](_buff);
			}
		};

		class MsgBuilder : public Msg
		{
		private:
			MsgBuilder() {}
		public:
			/* Static */
			/* Creates a Msg from the buffer.  This is used to build an Msg from a buffer and read it without
			* any modification to the underlying data.  I.E. You have an array of data you received from a slave/client, and you
			* wish to parse that data, simply call this function and pass the returned value to the copy constructor of the object
			* you wish to cast to. */
			static MsgBuilder fromBuff(void* buff, size_t buffSize = 0, bool own = false)
			{
				MsgBuilder msg{};
				msg._init(buff, buffSize, own);

				return(msg);
			}
			/**********/

			/* Methods */
				/* Virtual */
			virtual bool isValid()const { return(true); }
			virtual size_t getUtilizedSize()const { return(0); }
				/***********/
			/***********/
		};

		template<size_t BUFF_SIZE>
		class MsgStaticSized : public Msg
		{
		protected:
			MsgStaticSized() : Msg() {}
			MsgStaticSized(void* buff, bool own = false) : Msg(buff, BUFF_SIZE, own) {}

		public:			
			MsgStaticSized(const Msg& msg, bool referenceBuff = true) : Msg(msg, BUFF_SIZE, referenceBuff){}

			/* Getters */
			static constexpr size_t getBuffSize() { return(BUFF_SIZE); }
			/***********/

			/* Virtual */
				/* Getters */
			virtual bool isValid()const 
			{ 
				uint8_t buffSize = (uint8_t)Msg::getBuffSize();
				return(!buffSize || buffSize == BUFF_SIZE); 
			}
				/***********/
			/***********/
		};
	}
}

#endif