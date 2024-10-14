#include "Thread.h"

#include <gtest/gtest.h>

TEST(THREAD, TasksCompleteSuccessfully_test)
{
    bondev::Thread thread;

    EXPECT_EQ(bondev::WorkStatus::Waiting, thread.getWorkStatus());

    thread.giveTask(
        []()
        {
            std::cout << "TASK 1 << \n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    thread.giveTask(
        []()
        {
            std::cout << "TASK 2 << \n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    thread.runThread();

    EXPECT_EQ(bondev::WorkStatus::InProgress, thread.getWorkStatus());
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_EQ(bondev::WorkStatus::Done, thread.getWorkStatus());
}

TEST(THREAD, ForcedThreadStop_test)
{
    bondev::Thread thread;

    EXPECT_EQ(bondev::WorkStatus::Waiting, thread.getWorkStatus());

    thread.giveTask(
        []()
        {
            std::cout << "TASK 1 \n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    thread.giveTask(
        []()
        {
            std::cout << "TASK 2 \n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });

    thread.runThread();

    EXPECT_EQ(bondev::WorkStatus::InProgress, thread.getWorkStatus());
    thread.stopThread();
    EXPECT_EQ(bondev::WorkStatus::Done, thread.getWorkStatus());
}

TEST(THREAD, ThreadSyncType_test)
{
    // TODO: I need to add a check that the thread has finished working with the join() flag and
    // detach() correctly and without errors
    bondev::Thread thread_detach;

    thread_detach.setThreadSyncType(bondev::ThreadSyncType::Detach);

    thread_detach.giveTask(
        []()
        {
            for (size_t i = 0; i < 100; i++)
            {
                std::cout << "DETACH TEST\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    thread_detach.runThread();
    std::this_thread::sleep_for(std::chrono::seconds(4));
}
