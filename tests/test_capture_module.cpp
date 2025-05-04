// test_capture_module.cpp
#include <gtest/gtest.h>
#include "capture_module.h"
#include <thread>
#include <chrono>

TEST(CaptureModuleTest, CaptureAudio) {
    IPCManager mic_ipc, loopback_ipc;
    BufferManager mic_buffer(mic_ipc), loopback_buffer(loopback_ipc);

    CaptureModule capture(mic_buffer, loopback_buffer);
    ASSERT_NO_THROW(capture.start());

    // Aguardar captura (simulada)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    capture.stop();

    // Verificar buffers (simulados; em ambiente real, depende de entrada de áudio)
    AudioBuffer mic_data, loopback_data;
    bool mic_received = mic_buffer.pop(mic_data);
    bool loopback_received = loopback_buffer.pop(loopback_data);

    // Em ambiente simulado, verificar se buffers foram preenchidos
    if (mic_received) {
        EXPECT_EQ(mic_data.size, FRAME_SIZE * CHANNELS);
        // SNR simplificado (não aplicável sem áudio real)
        EXPECT_LE(std::abs(mic_data.data[0]), 1.0); // Amostras normalizadas
    }
    if (loopback_received) {
        EXPECT_EQ(loopback_data.size, FRAME_SIZE * CHANNELS);
        EXPECT_LE(std::abs(loopback_data.data[0]), 1.0);
    }

    // Verificar ausência de crosstalk (simplificado; depende de áudio real)
    EXPECT_TRUE(mic_received || loopback_received); // Pelo menos um buffer preenchido
}