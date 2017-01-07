#include "includes/TaskPool.h"

namespace alib
{
	TaskPool::TaskPool() {}
	TaskPool::TaskPool(size_t loopTime) :minLoopTime(loopTime) {}

	/****Getters****/
	DList* TaskPool::getTasks() const { return(tasks); }
	size_t TaskPool::getTaskCount() const { return(DList_get_count(tasks)); }
	/***************/

	/****Setters****/
	void TaskPool::setMinLoopTime(size_t millis)
	{
		minLoopTime = millis;
	}
	/***************/

	void TaskPool::addTask(Task& task, bool freeOnRemove)
	{
		addTask(&task, freeOnRemove);
	}
	void TaskPool::addTask(Task* task, bool freeOnRemove)
	{
		alib_free_value free_val = (freeOnRemove)?Task::free:task->getFreeArgCb();
		DListItem* itm = newDListItem(task, free_val, NULL);
		DList_push_back(tasks, itm);
	}
	void TaskPool::removeTask(Task& task)
	{
		removeTask(&task);
	}
	void TaskPool::removeTask(Task* task)
	{
		DList_remove_item_lsafe(tasks,
			(DListItem*)DList_get_by_value(tasks, task));
	}

	void TaskPool::loop()
	{
#ifdef __linux__
		struct timeval startTime;
		gettimeofday(&startTime, NULL);
#else
#ifdef ARDUINO
		unsigned long startTime = millis();
#else
		/* We don't support this system's timing, only loop counter will be used. */
#endif
#endif

		/* Run Tasks */
		DListItem* iter = (DListItem*)DList_get(tasks, 0);
		for (; iter; iter = DListItem_get_next_item(iter))
		{
			Task* task = (Task*)DListItem_get_value(iter);
			task->loop();
		}
		DList_remove_marked_items(tasks);
		/*************/

		/* Check time. */
#ifdef __linux__
		struct timeval endTime;
		gettimeofday(&endTime, NULL);
		endTime.tv_sec -= startTime.tv_sec;
		if(endTime.tv_usec < startTime.tv_usec)
			endTime.tv_usec += MICROS_PER_SECOND - startTime.tv_usec;
		else
			endTime.tv_usec -= startTime.tv_usec;

		unsigned long long micros = SECONDS_TO_MICROS(endTime.tv_sec) +
			endTime.tv_usec;

		if(micros < MILLIS_TO_MICROS(minLoopTime))
			usleep(micros);
		
#else
#ifdef ARDUINO

		unsigned long endTime = millis();
		
		if (endTime < startTime)
			endTime += ULONG_MAX - startTime;
		else
			endTime -= startTime;

		if (endTime < minLoopTime)
			delay(minLoopTime - endTime);
#else
		/* We don't support this system's timing, only loop counter will be used. */
#endif
#endif
		/***************/
	}

	TaskPool::~TaskPool() { delDList(&tasks); }
}
