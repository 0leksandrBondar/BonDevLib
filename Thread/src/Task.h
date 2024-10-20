#pragma once

#include <atomic>
#include <functional>

namespace bondev
{

    enum class TaskStatus
    {
        Waiting,
        Started,
        Complited
    };

    enum class TaskPriority
    {
        Low,
        Normal,
        High
    };

    class Task
    {
    public:
        Task() = default;
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        template <typename Func, typename... Args>
        Task(Func&& func, Args&&... args)
            : _task(std::bind(std::forward<Func>(func), std::forward<Args>(args)...))
        {
        }

        Task(Task&& other) noexcept
            : _task(std::move(other._task)),
              _taskStatus(other._taskStatus.load()),
              _taskPriority(other._taskPriority.load())
        {
        }

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

        void execute() { _task(); }

        void setTaskPriority(const TaskPriority priority) { _taskPriority = priority; }

        [[nodiscard]] TaskStatus getTaskStatus() const { return _taskStatus.load(); }
        [[nodiscard]] TaskPriority getTaskPriority() const { return _taskPriority.load(); }

    private:
        std::function<void()> _task;
        std::atomic<TaskStatus> _taskStatus{ TaskStatus::Waiting };
        std::atomic<TaskPriority> _taskPriority{ TaskPriority::Low };
    };
} // namespace bondev