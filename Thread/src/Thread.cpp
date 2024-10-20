#include "Thread.h"

namespace bondev
{
    template <typename Func, typename... Args>
    Thread::Thread(bondev::TaskPriority priority, Func&& func, Args&&... args)
    {
        auto task = std::make_shared<Task>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        task->setTaskPriority(priority);
        _tasks.push(std::move(task));
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
                auto currentTask = std::move(_tasks.front());
                _tasks.pop();
                currentTask->execute();
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
