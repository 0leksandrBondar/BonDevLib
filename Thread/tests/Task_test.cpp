#include "Task.h"

#include <gtest/gtest.h>

TEST(TASK, Task_test)
{
    bondev::Task lowPriorityTask([]() { std::cout << "Task LOW PRIORITY executed" << std::endl; });
    bondev::Task NormalPriorityTask([]()
                                    { std::cout << "Task NORMAL PRIORITY executed" << std::endl; },
                                    bondev::TaskPriority::Normal);
    bondev::Task highPriorityTask([]()
                                  { std::cout << "Task  HIGH PRIORITY executed" << std::endl; },
                                  bondev::TaskPriority::High);

    lowPriorityTask.execute();
    NormalPriorityTask.execute();
    highPriorityTask.execute();

    EXPECT_EQ(bondev::TaskPriority::Low, lowPriorityTask.getTaskPriority());
    EXPECT_EQ(bondev::TaskPriority::High, highPriorityTask.getTaskPriority());
    EXPECT_EQ(bondev::TaskPriority::Normal, NormalPriorityTask.getTaskPriority());
}