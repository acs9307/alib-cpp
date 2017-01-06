#include "Task.h"

namespace alib
{
	/****Initializers****/
	void Task::init(task_callback callback, void* taskArg, alib_free_value freeArg,
		uint8_t loopsPerCallback)
	{
		tcb = callback;
		arg = taskArg;
		freeArgCb = freeArg;
		loopsPerCall = loopsPerCallback;

		/* Default init other members. */
		loopCount = 0;
	}
	/********************/

	/****Constructors****/
	Task::Task() {}
	Task::Task(task_callback callback, void* taskArg, alib_free_value freeArg) :
		tcb(callback), arg(taskArg), freeArgCb(freeArg) {}
	Task::Task(task_callback callback, void* taskArg, alib_free_value freeArg,
		uint8_t loopsPerCallback) :
		tcb(callback), arg(taskArg), freeArgCb(freeArg), loopsPerCall(loopsPerCallback) {}
	/********************/

	/****Getters****/
	task_callback Task::getCallback() const { return(tcb); }
	void* Task::getArg()const { return(arg); }
	alib_free_value Task::getFreeArgCb()const { return(freeArgCb); }
	uint8_t Task::getLoopsPerCall() const { return(loopsPerCall); }
	/***************/

	/****Setters****/
	void Task::setCallback(task_callback callback) { tcb = callback; }
	void Task::setArg(void* taskArg, alib_free_value freeTaskArg)
	{
		if (arg && freeArgCb)
			freeArgCb(arg);
		arg = taskArg;
		freeArgCb = freeTaskArg;
	}
	void Task::setLoopsPerCall(uint8_t loopsPerCallback)
	{
		loopsPerCall = loopsPerCallback;
		if (loopCount > loopsPerCall)
			loopCount = loopsPerCall;
	}
	/***************/

	void Task::loop()
	{
		if (!loopCount)
		{
			tcb(*this);
			loopCount = loopsPerCall;
		}
		else
			--loopCount;

	}

	void Task::free(void* _task)
	{
		if(!_task)return;
		
		Task* task = (Task*)_task;
		delete(task);
	}
	Task::~Task()
	{
		if (arg && freeArgCb)
			freeArgCb(arg);
	}
}
