// test_buffer_manager.cpp
#include <gtest/gtest.h>
#include "buffer_manager.h"

TEST(BufferManagerTest, BufferOperations) {
    IPCManager ipc;
    BufferManager buffer_manager(ipc);

    // Criar buffer simulado (20ms a 48kHz, 2 canais)
    AudioBuffer buffer;
    buffer.data.resize(960 * 2, 0.1f);
    buffer.size = 960 * 2;
    buffer.timestamp = 0.0;

    // Enfileirar 50 buffers (1s total)
    for (size_t i = 0; i < 50; ++i) {
        buffer_manager.push(buffer);
    }
    EXPECT_NEAR(buffer_manager.getBufferMs(), 1000.0, 1.0); // 1s

    // Desfileirar 25 buffers (500ms)
    AudioBuffer received;
    size_t received_count = 0;
    for (size_t i = 0; i < 25; ++i) {
        if (buffer_manager.pop(received)) {
            ++received_count;
            EXPECT_EQ(received.size, 960 * 2);
            EXPECT_FLOAT_EQ(received.data[0], 0.1f);
        }
    }
    EXPECT_EQ(received_count, 25);
    EXPECT_NEAR(buffer_manager.getBufferMs(), 500.0, 1.0); // 500ms

    // Verificar sem underruns
    for (size_t i = 0; i < 25; ++i) {
        EXPECT_TRUE(buffer_manager.pop(received));
    }
    EXPECT_FALSE(buffer_manager.pop(received)); // Fila vazia
    EXPECT_NEAR(buffer_manager.getBufferMs(), 0.0, 1.0);
}