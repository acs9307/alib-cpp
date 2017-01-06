#ifndef REFERENCE_COUNTER_IS_DEFINED
#define REFERENCE_COUNTER_IS_DEFINED

#ifndef ARDUINO
#include <iostream>
#else
#include <stdio.h>
#endif

namespace alib
{
	/* Object that keeps track of references to a specific object. 
	 * Useful as a base class. */
	class ReferenceCounter
	{
	private:
		size_t refCount;

	protected:
		/* Getters */
		/* Returns the number of reference that exist for the object. */
		size_t getRefCount()const;
		/***********/

		/* Decrements the reference count, if possible. */
		size_t decRef();

	public:
		/* Increments the reference counter and returns a pointer to the current object. */
		void* getRef();
	};
}
#endif
