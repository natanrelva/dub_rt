// test_decoder.cpp
#include <gtest/gtest.h>
#include "decoder.h"
#include <chrono>

TEST(DecoderTest, DecodeFrame) {
    Decoder decoder;
    // Simular quadro Opus (dados fictícios)
    std::vector<unsigned char> input(100, 0); // Tamanho típico de quadro comprimido
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<float> output;
    ASSERT_NO_THROW(output = decoder.decode(input));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Verificar latência e tamanho
    EXPECT_LT(duration / 1000.0, 5.0); // <5ms
    EXPECT_LE(output.size(), FRAME_SIZE * CHANNELS);

    // Verificar fidelidade (simplificado, já que entrada é fictícia)
    for (float sample : output) {
        EXPECT_LE(std::abs(sample), 1.0); // Amostras PCM normalizadas
    }
}