#pragma once

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
        template <typename Func, typename... Args>
        Task(Func&& func, Args&&... args)
            : _task(std::bind(std::forward<Func>(func), std::forward<Args>(args)...))
        {
        }

        void execute() { _task(); }

        void setTaskPriority(const TaskPriority priority) { _taskPriority = priority; }

        [[nodiscard]] TaskStatus getTaskStatus() const { return _taskStatus; }
        [[nodiscard]] TaskPriority getTaskPriority() const { return _taskPriority; }

    private:
        std::function<void()> _task;
        TaskStatus _taskStatus{ TaskStatus::Waiting };
        TaskPriority _taskPriority{ TaskPriority::Low };
    };
} // namespace bondev