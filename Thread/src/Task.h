#pragma once

#include <atomic>
#include <functional>
#include <string>

namespace bondev
{

    enum class TaskStatus
    {
        Waiting,
        Started,
        Completed
    };

    enum class TaskPriority
    {
        Low,
        Normal,
        High
    };

    class Task final
    {
    public:
        Task() = default;
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        template <typename Func, typename... Args>
        explicit Task(Func&& func, Args&&... args)
            : _task{ [&]() { std::invoke(std::forward<Func>(func), std::forward<Args>(args)...); } }
        {
        }

        Task(Task&& other) noexcept { *this = std::move(other); }

        Task& operator=(Task&& other) noexcept
        {
            if (this != &other)
            {
                _task = std::move(other._task);
                _taskStatus.store(other._taskStatus.load());
                _taskPriority.store(other._taskPriority.load());
            }
            return *this;
        }

        void execute()
        {
            _taskStatus = TaskStatus::Started;
            _task();
            _taskStatus = TaskStatus::Completed;
        }

        void setTaskName(const std::string& taskName) { _taskName = taskName; }
        void setTaskPriority(const TaskPriority priority) { _taskPriority = priority; }

        [[nodiscard]] std::string getTaskName() const { return _taskName; }
        [[nodiscard]] TaskStatus getTaskStatus() const { return _taskStatus.load(); }
        [[nodiscard]] TaskPriority getTaskPriority() const { return _taskPriority.load(); }

    private:
        std::string _taskName;
        std::function<void()> _task;
        std::atomic<TaskStatus> _taskStatus{ TaskStatus::Waiting };
        std::atomic<TaskPriority> _taskPriority{ TaskPriority::Low };
    };
} // namespace bondev