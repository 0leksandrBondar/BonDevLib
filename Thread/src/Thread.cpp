#include "Thread.h"

namespace bondev
{
	Thread::Thread(const std::string& name, ThreadSyncType syncType) : _threadName{ name }, _syncType{ syncType }{}

	Thread::~Thread()
	{ 
		stopThread();
	}

	void Thread::runThread()
	{
        _stop = false;
        _workStatus = WorkStatus::InProgress;
		_thread = std::jthread([this]() { runTasks(); });
	}

	void Thread::runTasks()
	{
        _threadId = std::this_thread::get_id();
		while (!_stop)
		{
			if (!_tasks.empty())
			{
				auto currentTask = std::move(_tasks.front());
				_tasks.pop();
				currentTask();
			}
			else
			{
				_stop = true;
                _workStatus = WorkStatus::Done;
			}
		}
	}

	void Thread::stopThread()
	{
		_stop = true;
        _workStatus = WorkStatus::Done;
        if (_syncType != ThreadSyncType::Detach && _thread.joinable())
            _thread.join();
    }

    void Thread::waitForAll()
	{

	}

} // namespace bondev

