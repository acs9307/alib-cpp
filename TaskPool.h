#ifndef TASK_POOL_IS_DEFINED
#define TASK_POOL_IS_DEFINED

#ifdef ARDUINO
#include <Arduino.h>
#endif

extern "C"
{
#include <limits.h>

#ifdef ARDUINO
#include <alib-c/includes/DList.h>
#include <alib-c/includes/alib_time.h>
#else
#include <alib-c/DList.h>
#include <alib-c/alib_time.h>
#endif
}

#ifdef __linux__
extern "C"
{
#include <unistd.h>
#include <sys/time.h>
}
#endif

#include "Task.h"

namespace alib
{
	class TaskPool
	{
	private:
		DList* tasks = newDList();
		size_t minLoopTime = 0;
	public:
		TaskPool();
		TaskPool(size_t loopTime);

		/****Getters****/
		DList* getTasks() const;
		size_t getTaskCount() const;
		/***************/

		/****Setters****/
		void setMinLoopTime(size_t millis);
		/***************/

		void addTask(Task& task, bool freeOnRemove);
		void addTask(Task* task, bool freeOnRemove);
		void removeTask(Task& task);
		void removeTask(Task* task);

		void loop();

		~TaskPool();
	};
}
#endif
