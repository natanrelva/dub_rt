// ipc_manager.h
#ifndef IPC_MANAGER_H
#define IPC_MANAGER_H

#include <queue>
#include <mutex>
#include <vector>

struct AudioBuffer {
    std::vector<float> data;
    size_t size;
    double timestamp;
};

class IPCManager {
private:
    std::queue<AudioBuffer> queue_;
    std::mutex mutex_;

public:
    void push(const AudioBuffer& buffer);
    bool pop(AudioBuffer& buffer);
};

#endif // IPC_MANAGER_H

// ipc_manager.cpp
#include "ipc_manager.h"

void IPCManager::push(const AudioBuffer& buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(buffer);
}

bool IPCManager::pop(AudioBuffer& buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!queue_.empty()) {
        buffer = queue_.front();
        queue_.pop();
        return true;
    }
    return false;
}

// test_ipc_manager.cpp
#include <gtest/gtest.h>
#include "ipc_manager.h"
#include <chrono>

TEST(IPCManagerTest, PushPopPerformance) {
    IPCManager ipc;
    const size_t buffer_size = 960 * 2; // 20ms a 48kHz, 2 canais
    const size_t num_buffers = 1000;

    // Criar buffer simulado
    AudioBuffer buffer;
    buffer.data.resize(buffer_size, 0.1f);
    buffer.size = buffer_size;
    buffer.timestamp = 0.0;

    // Medir tempo de push
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_buffers; ++i) {
        ipc.push(buffer);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto push_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Medir tempo de pop
    start = std::chrono::high_resolution_clock::now();
    AudioBuffer received;
    size_t received_count = 0;
    for (size_t i = 0; i < num_buffers; ++i) {
        if (ipc.pop(received)) {
            ++received_count;
            EXPECT_EQ(received.size, buffer_size);
            EXPECT_EQ(received.data.size(), buffer_size);
            EXPECT_FLOAT_EQ(received.data[0], 0.1f);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto pop_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Verificar taxa e latência
    double total_bytes = num_buffers * buffer_size * sizeof(float);
    double push_rate = total_bytes / (push_duration / 1e6) / 1e6; // MB/s
    double pop_rate = total_bytes / (pop_duration / 1e6) / 1e6; // MB/s
    double avg_latency = (push_duration + pop_duration) / (double)num_buffers / 1e3; // ms

    EXPECT_GE(push_rate, 20.0);
    EXPECT_GE(pop_rate, 20.0);
    EXPECT_LT(avg_latency, 1.0);
    EXPECT_EQ(received_count, num_buffers);
}