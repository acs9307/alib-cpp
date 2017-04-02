#ifndef ALIB_COMM_MSG_WRAPPER_IS_DEFINED
#define ALIB_COMM_MSG_WRAPPER_IS_DEFINED

/* NOTE:
* THIS IS AN HPP FILE BECAUSE DEFINING VIRTUAL FUNCTIONS IN DIFFERENT FILE CAUSES LINKER ERRORS WITH TEMPLATED CLASSES.... */

namespace alib
{
	namespace comm
	{
		template<class BASE, size_t HEADER_SIZE>
		class MsgWrapper : public BASE
		{
			/* Should we check that BASE is a valid IMsg object??? */
		protected:
			/* Constructors */
			MsgWrapper() {}
			/****************/

			/* Functions */
				/* Getters */
			static size_t _getHeaderSize() { return(HEADER_SIZE); }
				/***********/
			/*************/

			/* Virtual Functions */
				/* Getters */
			/* Gets the value at the given offset related to the derived buffer.
			*
			* Parameters:
			*		offset: The offset, in bytes, at which to retrieve the value in the message buffer. */
			template<typename T>
			T _getDerivedOffset(size_t offset) const
			{
				
#ifdef ARDUINO
				return(*((T*)_getDerivedOffset(offset)));
#else
				return(BASE::_getDerivedOffset<T>(offset + HEADER_SIZE));
#endif
			}
			/* Gets the value at the given offset to the specified value.
			 * Use the above functions unless you have a shitty compiler like the Arduino one which does not allow 
			 * 		recursive template calls...
			 *
			 * Parameters:
			 *		offset: The offset, in bytes, at which to retrieve the value in the message buffer. */
			const void* _getDerivedOffset(size_t offset)
			{
				return(BASE::_getDerivedOffset(HEADER_SIZE + offset));
			}

			/* Returns the size specified as the derived size in the template of the object. */
			virtual size_t _getDerivedSize() const { return(BASE::_getDerivedSize() - HEADER_SIZE); }
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
				#ifdef ARDUINO
				_setDerivedOffset(offset, &value, sizeof(value));
				#else
				BASE::_setDerivedOffset<T>(offset + HEADER_SIZE, value);
				#endif
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
				offset += HEADER_SIZE;
				BASE::_setDerivedOffset(offset, value, valueSize);
			}
				/***********/
			/*********************/
		};
	}
}

#endif
