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
			static constexpr size_t _getHeaderSize() { return(HEADER_SIZE); }
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
				return(BASE::template _getDerivedOffset<T>(offset + HEADER_SIZE));
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
				BASE::template _setDerivedOffset<T>(offset + HEADER_SIZE, value);
			}
				/***********/
			/*********************/
		};
	}
}

#endif
