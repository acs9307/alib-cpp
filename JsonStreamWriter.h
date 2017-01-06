#ifndef JSON_STREAM_WRITER_IS_DEFINED
#define JSON_STREAM_WRITER_IS_DEFINED

#include <Arduino.h>

namespace alib
{
	class JsonStreamWriter
	{
		/* Members */
		Stream& _stream;
		bool _itemWritten = false;
		/***********/

		/* Static Functions */
		void writeObjectHeader(const char* name);
		void writeSeperator();
		/********************/

	public:
		/* Constructors */
		JsonStreamWriter(Stream& stream);
		/****************/

		/* Functions */
		void begin();
		void end();

		void writeObject(const char* name, const char* value);
		void writeObject(const char* name, int value);
		void writeObject(const char* name, bool value);
		/*************/
	};
}
#endif
