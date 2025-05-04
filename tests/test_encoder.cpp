// test_encoder.cpp
#include <gtest/gtest.h>
#include "encoder.h"
#include <chrono>

TEST(EncoderTest, EncodeFrame) {
    Encoder encoder;
    std::vector<float> input(FRAME_SIZE * CHANNELS, 0.1f); // Áudio simulado
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned char> output;
    ASSERT_NO_THROW(output = encoder.encode(input));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Verificar latência
    EXPECT_LT(duration / 1000.0, 5.0); // <5ms

    // Verificar taxa (aproximada)
    double bitrate = (output.size() * 8.0) / (FRAME_SIZE / (double)SAMPLE_RATE); // bits/s
    EXPECT_NEAR(bitrate / 1000.0, 24.0, 5.0); // ~24 kbps
}