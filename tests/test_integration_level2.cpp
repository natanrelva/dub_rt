// test_integration_level2.cpp
#include <gtest/gtest.h>
#include "capture_module.h"
#include "output_module.h"
#include "scheduler.h"
#include <chrono>

TEST(Level2IntegrationTest, CaptureToOutput) {
    IPCManager mic_ipc, loopback_ipc, local_ipc, meets_ipc;
    BufferManager mic_buffer(mic_ipc), loopback_buffer(loopback_ipc);
    BufferManager local_buffer(local_ipc), meets_buffer(meets_ipc);

    CaptureModule capture(mic_buffer, loopback_buffer);
    OutputModule output(local_buffer, meets_buffer);
    Scheduler scheduler;

    // Simular áudio de entrada
    std::vector<float> input(FRAME_SIZE * CHANNELS, 0.1f);
    AudioBuffer input_buffer;
    input_buffer.data = input;
    input_buffer.size = input.size();
    input_buffer.timestamp = 0.0;

    auto start = std::chrono::high_resolution_clock::now();

    // Escalonar captura
    scheduler.scheduleTask([&]() {
        mic_buffer.push(input_buffer); // Simular microfone
        loopback_buffer.push(input_buffer); // Simular loopback
    }, 20);

    // Escalonar saída
    scheduler.scheduleTask([&]() {
        AudioBuffer local_data, meets_data;
        if (mic_buffer.pop(local_data)) {
            local_buffer.push(local_data);
        }
        if (loopback_buffer.pop(meets_data)) {
            meets_buffer.push(meets_data);
        }
        output.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        output.stop();
    }, 10);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Verificar latência e integridade
    EXPECT_LT(duration / 1000.0, 20.0); // <20ms
    AudioBuffer dummy;
    EXPECT_FALSE(local_buffer.pop(dummy)); // Buffer consumido
    EXPECT_FALSE(meets_buffer.pop(dummy)); // Buffer consumido
}