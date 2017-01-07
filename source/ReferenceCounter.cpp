#include "includes/ReferenceCounter.h"

namespace alib
{
	/* Protected */
		/* Getters */
	/* Returns the number of reference that exist for the object. */
	size_t ReferenceCounter::getRefCount()const { return(refCount); }
		/***********/

	/* Decrements the reference count, if possible. */
	size_t ReferenceCounter::decRef() { return((refCount) ? --refCount : refCount); }
	/*************/

	/* Public */
	/* Increments the reference counter and returns a pointer to the current object. */
	void* ReferenceCounter::getRef()
	{
		++refCount;
		return((void*)this);
	}
	/**********/
}
