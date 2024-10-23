#include "Thread.h"

namespace bondev
{
    template <typename Func, typename... Args>
    Thread::Thread(TaskPriority priority, Func&& func, Args&&... args)
    {
        giveTask(priority, std::forward<Func>(func), std::forward<Args>(args)...);
    }

    Thread& Thread::operator=(Thread&& other) noexcept
    {
        // TODO: Add tests for move semantics

        if (this != &other)
        {
            _threadId = std::move(other._threadId);
            _stop = other._stop;
            _thread = std::move(other._thread);
            _threadName = std::move(other._threadName);
            _tasks = std::move(other._tasks);
            _workStatus = other._workStatus;
            _syncType = other._syncType;

            // Reset the state of the moved-from object
            other._stop = false;
            other._workStatus = WorkStatus::Waiting;
        }
        return *this;
    }

    Thread::~Thread() { stopThread(); }

    void Thread::runThread()
    {
        _stop = false;
        _workStatus = WorkStatus::InProgress;
        _thread = std::jthread([this]() { runTasks(); });

        if (_syncType == ThreadSyncType::Detach)
            _thread.detach();
    }

    void Thread::runTasks()
    {
        _threadId = std::this_thread::get_id();
        while (!_stop)
        {
            if (!_tasks.empty())
            {
                _tasks.front()->execute();
                _tasks.pop();
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

} // namespace bondev
