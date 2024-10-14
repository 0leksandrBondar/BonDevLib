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
    thread.stopThread();
    EXPECT_EQ(bondev::WorkStatus::Done, thread.getWorkStatus());
}