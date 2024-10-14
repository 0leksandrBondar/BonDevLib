#pragma once

#include <queue>
#include <thread>
#include <functional>

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
		explicit Thread(const std::string& name, ThreadSyncType syncType = ThreadSyncType::Join);

        template <typename Func, typename... Args>
        Thread(Func&& func, Args&&... args);

		virtual ~Thread();

		void runThread();
        void stopThread();

		void waitForAll();

		template <typename Func, typename... Args>
        void giveTask(Func&& func, Args&&... args);

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
		std::queue<std::function<void()>> _tasks;
        WorkStatus _workStatus{ WorkStatus::Waiting };
		ThreadSyncType _syncType{ ThreadSyncType::Join };
	};

	template <typename Func, typename... Args>
    Thread::Thread(Func&& func, Args&&... args)
    {
        auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        _tasks.push(task);
    }

	template <typename Func, typename... Args>
    void Thread::giveTask(Func&& func, Args&&... args)
    {
        auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        _tasks.push(task);
    }
}