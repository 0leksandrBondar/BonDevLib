#pragma once

#include "Task.h"

#include <queue>
#include <functional>
#include <memory>
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

    class Thread final
    {
        using ThreadId = std::thread::id;

    public:
        Thread() = default;
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        template <typename Func, typename... Args>
        Thread(TaskPriority priority, Func&& func, Args&&... args);

        Thread(Thread&& other) noexcept { *this = std::move(other); }

        Thread& operator=(Thread&& other) noexcept;

        virtual ~Thread();

        void runThread();
        void stopThread();

        template <typename Func, typename... Args>
        std::shared_ptr<Task> giveTask(TaskPriority priority, Func&& func, Args&&... args)
        {
            auto task
                = std::make_shared<Task>(std::forward<Func>(func), std::forward<Args>(args)...);
            task->setTaskPriority(priority);
            _tasks.push(task);
            return task;
        }

        void setThreadName(const std::string& name) { _threadName = name; }
        void setThreadSyncType(const ThreadSyncType syncType) { _syncType = syncType; }

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
        std::queue<std::shared_ptr<Task>> _tasks;
        WorkStatus _workStatus{ WorkStatus::Waiting };
        ThreadSyncType _syncType{ ThreadSyncType::Join };
    };
} // namespace bondev