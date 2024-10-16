#pragma once

#include <functional>

namespace bondev
{
    class Task
    {
    public:
        template <typename Func, typename... Args>
        Task(Func&& func, Args&&... args)
            : _task(std::bind(std::forward<Func>(func), std::forward<Args>(args)...))
        {
        }

        void execute() { _task(); }

    private:
        std::function<void()> _task;
    };
} // namespace bondev