#ifndef ALIB_MEMORY_IS_DEFINED
#define ALIB_MEMORY_IS_DEFINED

#ifdef ARDUINO
#include <new.h>
#endif

namespace alib
{
#ifndef shared_ptr
#define shared_ptr
	/* HOW DO WE DETERMINE HOW TO DELETE DATA, EITHER 'delete()' or 'delete[]()'?*/
	template<typename T>
	class shared_ptr
	{
	private:
		T* _data = NULL;
		size_t* _refCount = NULL;

		/* Functions */
		void _copy(const shared_ptr& ptr)
		{
			~shared_ptr();

			_refCount = ptr._refCount;
			++(*(_refCount));
			_data = ptr._data;
		}
		/*************/

	public:
		/* Constructors */
		shared_ptr(T* data)
		{
			_refCount = new size_t;
			_data = data;
		}

		shared_ptr(const shared_ptr& ptr)
		{
			_copy(ptr);
		}
		/****************/

		~shared_ptr()
		{
			if (!_refCount)
			{
				if (_data)
					delete(_data);
			}
			else
			{
				--(*_refCount);
				if (!*_refCount)
				{
					delete(_data);
					delete(_refCount);
				}
			}
		}

		/* Overloaded Operators */
		T& operator*()
		{
			return(*_data);
		}

		void operator+(int count)
		{
			_data += count;
		}
		/************************/
	};
#endif
}

#endif