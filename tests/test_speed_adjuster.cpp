// test_speed_adjuster.cpp
#include <gtest/gtest.h>
#include "speed_adjuster.h"
#include <cmath>

TEST(SpeedAdjusterTest, SpeedAdjustment) {
    SpeedAdjuster adjuster;
    AudioBuffer buffer;
    buffer.data.resize(960 * 2, 0.1f); // 20ms a 48kHz
    buffer.size = 960 * 2;
    buffer.timestamp = 0.0;

    // Testar velocidades: 0.9x (<200ms), 1.0x (200ms-1s), 1.1x (>1s)
    struct TestCase {
        double buffer_ms;
        float expected_speed;
        double expected_size_factor;
    };
    std::vector<TestCase> cases = {
        {100, 0.9f, 1.111}, // 1/0.9
        {500, 1.0f, 1.0},
        {1500, 1.1f, 0.909} // 1/1.1
    };

    for (const auto& tc : cases) {
        AudioBuffer test_buffer = buffer;
        adjuster.adjust(test_buffer, tc.buffer_ms);
        EXPECT_FLOAT_EQ(test_buffer.speed_factor, tc.expected_speed);
        EXPECT_NEAR(test_buffer.size / (double)buffer.size, tc.expected_size_factor, 0.1);

        // Verificar SNR (simplificado)
        double mse = 0.0;
        for (size_t i = 0; i < std::min(test_buffer.data.size(), buffer.data.size()); ++i) {
            mse += std::pow(test_buffer.data[i] - 0.1f, 2);
        }
        mse /= std::min(test_buffer.data.size(), buffer.data.size());
        double snr = 10 * std::log10(0.1 * 0.1 / mse);
        EXPECT_GT(snr, 30.0);
    }
}