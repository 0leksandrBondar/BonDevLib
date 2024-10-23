#include "Task.h"

#include <gtest/gtest.h>
#include <thread>

TEST(TASK, execution_test)
{
    bondev::Task task(
        []()
        {
            for (int i = 0; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });

    const std::string expectedTaskName{ "TaskName" };
    constexpr auto priority = bondev::TaskPriority::Normal;

    task.setTaskPriority(priority);
    task.setTaskName(expectedTaskName);

    EXPECT_EQ(priority, task.getTaskPriority());
    EXPECT_EQ(expectedTaskName, task.getTaskName());
    EXPECT_EQ(bondev::TaskStatus::Waiting, task.getTaskStatus());

    task.execute();

    EXPECT_EQ(bondev::TaskStatus::Completed, task.getTaskStatus());
}

TEST(TASK, TestBehaviorTaskInThread)
{
    bondev::Task task(
        []()
        {
            for (int i = 0; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    EXPECT_EQ(bondev::TaskStatus::Waiting, task.getTaskStatus());
    std::thread thread([&]() { task.execute(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(bondev::TaskStatus::Started, task.getTaskStatus());
    thread.join();
    EXPECT_EQ(bondev::TaskStatus::Completed, task.getTaskStatus());
}