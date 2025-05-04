// test_output_module.cpp
#include <gtest/gtest.h>
#include "output_module.h"
#include <thread>
#include <chrono>
#define FRAME_SIZE 1024
#define CHANNELS 2

TEST(OutputModuleTest, OutputAudio) {
    IPCManager local_ipc, meets_ipc;
    BufferManager local_buffer(local_ipc), meets_buffer(meets_ipc);

    // Preencher buffers com áudio simulado
    AudioBuffer buffer;
    buffer.data.resize(FRAME_SIZE * CHANNELS, 0.1f);
    buffer.size = FRAME_SIZE * CHANNELS;
    buffer.timestamp = 0.0;
    local_buffer.push(buffer);
    meets_buffer.push(buffer);

    OutputModule output(local_buffer, meets_buffer);
    ASSERT_NO_THROW(output.start());

    // Aguardar saída (simulada)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    output.stop();

    // Verificar que buffers foram consumidos
    AudioBuffer dummy;
    EXPECT_FALSE(local_buffer.pop(dummy)); // Buffer local consumido
    EXPECT_FALSE(meets_buffer.pop(dummy)); // Buffer meets consumido
}