// test_scheduler.cpp
#include <gtest/gtest.h>
#include "scheduler.h"
#include <chrono>
#include <vector>
#include <mutex>

TEST(SchedulerTest, PriorityScheduling) {
    Scheduler scheduler;
    std::vector<int> execution_order;
    std::mutex mutex;

    auto task = [&execution_order, &mutex](int id) {
        std::lock_guard<std::mutex> lock(mutex);
        execution_order.push_back(id);
    };

    // Escalonar 3 tarefas com prioridades 10, 20, 30
    auto start = std::chrono::high_resolution_clock::now();
    scheduler.scheduleTask([task]() { task(1); }, 10);
    scheduler.scheduleTask([task]() { task(2); }, 20);
    scheduler.scheduleTask([task]() { task(3); }, 30);

    // Aguardar execução
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_latency = duration / 3.0 / 1e3; // ms

    // Verificar ordem (maior prioridade primeiro)
    EXPECT_EQ(execution_order.size(), 3);
    EXPECT_EQ(execution_order[0], 3); // Prioridade 30
    EXPECT_EQ(execution_order[1], 2); // Prioridade 20
    EXPECT_EQ(execution_order[2], 1); // Prioridade 10
    EXPECT_LT(avg_latency, 1.0);
}