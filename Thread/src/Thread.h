#pragma once

#include "Task.h"

#include <queue>
#include <functional>
#include <thread>

namespace bondev
{
    enum class ThreadSyncType
    {
        Join,
        Detach
    };

    enum class WorkStatus
    {
        Done,
        Waiting,
        InProgress,
    };

    class Thread
    {
        using ThreadId = std::thread::id;

    public:
        Thread() = default;
        template <typename Func, typename... Args>
        Thread(Func&& func, Args&&... args, TaskPriority priority = TaskPriority::Low);

        virtual ~Thread();

        void runThread();
        void stopThread();

        template <typename Func, typename... Args>
        void giveTask(Func&& func, Args&&... args, TaskPriority priority = TaskPriority::Low);

        void setThreadName(const std::string& name) { _threadName = name; }
        void setThreadSyncType(ThreadSyncType syncType) { _syncType = syncType; }

        [[nodiscard]] WorkStatus getWorkStatus() const { return _workStatus; }
        [[nodiscard]] std::thread::id getThreadId() const { return _threadId; }
        [[nodiscard]] std::string getThreadName() const { return _threadName; }
        [[nodiscard]] std::size_t getCountOfTasks() const { return _tasks.size(); }

    private:
        void runTasks();

    private:
        ThreadId _threadId;
        bool _stop{ false };
        std::jthread _thread;
        std::string _threadName;
        std::queue<Task> _tasks;
        WorkStatus _workStatus{ WorkStatus::Waiting };
        ThreadSyncType _syncType{ ThreadSyncType::Join };
    };

    template <typename Func, typename... Args>
    void Thread::giveTask(Func&& func, Args&&... args, TaskPriority priority)
    {
        bondev::Task task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        task.setTaskPriority(priority);
        _tasks.push(task);
    }
} // namespace bondev