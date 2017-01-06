#ifndef TASK_IS_DEFINED
#define TASK_IS_DEFINED

#ifdef ARDUINO

extern "C"
{
#include <alib-c/includes/alib_types.h>
}
#include <new.h>

#else

extern "C"
{
#include <alib-c/alib_types.h>
}

#endif


extern "C"
{
#include <stdint.h>
}

namespace alib
{
	/* Prototypes */
	class Task;
	/**************/
	
	typedef void(*task_callback)(Task&);
	
	class Task
	{
	private:
		task_callback tcb = NULL;
		void* arg = NULL;
		alib_free_value freeArgCb = NULL;

		/* The number of loops that should be made before
		 * calling the callback. */
		uint8_t loopsPerCall = 1;
		/* The number of loops iterated through before calling the callback. */
		uint8_t loopCount = 0;

	public:
		/****Initializers****/
		void init(task_callback callback, void* taskArg, alib_free_value freeArg,
			uint8_t loopsPerCallback);
		/********************/

		/****Constructors****/
		Task();
		Task(task_callback callback, void* taskArg, alib_free_value freeArg);
		Task(task_callback callback, void* taskArg, alib_free_value freeArg,
			uint8_t loopsPerCallback);
		/********************/

		/****Getters****/
		task_callback getCallback() const;
		void* getArg()const;
		alib_free_value getFreeArgCb()const;
		uint8_t getLoopsPerCall() const;
		/***************/

		/****Setters****/
		void setCallback(task_callback callback);
		void setArg(void* taskArg, alib_free_value freeTaskArg);
		void setLoopsPerCall(uint8_t loopsPerCallback);
		/***************/

		void loop();

		
		static void free(void* _task);
		~Task();
	};
}
#endif
