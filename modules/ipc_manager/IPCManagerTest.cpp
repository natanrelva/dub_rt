#include <gtest/gtest.h>
#include <vector>
#include <chrono>
#include <thread>
#include "IPCManager.h"

// Configuração do buffer simulado
constexpr size_t FRAME_COUNT = 960; // 960 frames
constexpr size_t SAMPLE_SIZE = sizeof(float); // 4 bytes por amostra (mono)
constexpr size_t BUFFER_SIZE = FRAME_COUNT * SAMPLE_SIZE; // 3.84 KB por buffer
constexpr size_t NUM_BUFFERS = 1000; // Enviar 1000 buffers
constexpr double MIN_THROUGHPUT_MBPS = 20.0; // Critério: >20 MB/s

class IPCManagerTest : public ::testing::Test {
protected:
    IPCManager<std::vector<float>> ipc;
};

// Teste 1: Integridade dos dados
TEST_F(IPCManagerTest, DataIntegrity) {
    std::vector<float> input_buffer(FRAME_COUNT, 1.0f);
    std::vector<std::vector<float>> sent_buffers(NUM_BUFFERS, input_buffer);
    std::vector<std::vector<float>> received_buffers;
    received_buffers.reserve(NUM_BUFFERS);

    // Produtor
    std::thread producer([&]() {
        for (const auto& buffer : sent_buffers) {
            ipc.push(buffer);
        }
    });

    // Consumidor
    std::thread consumer([&]() {
        for (size_t i = 0; i < NUM_BUFFERS; ++i) {
            std::vector<float> received;
            ipc.pop_blocking(received);
            received_buffers.push_back(received);
        }
    });

    producer.join();
    consumer.join();

    ASSERT_EQ(received_buffers.size(), NUM_BUFFERS);
    for (size_t i = 0; i < NUM_BUFFERS; ++i) {
        EXPECT_EQ(received_buffers[i], sent_buffers[i]) << "Perda de dados no buffer " << i;
    }
}

// Teste 2: Taxa de transferência
TEST_F(IPCManagerTest, Throughput) {
    std::vector<float> input_buffer(FRAME_COUNT, 1.0f);
    std::vector<std::vector<float>> sent_buffers(NUM_BUFFERS, input_buffer);

    auto start_time = std::chrono::high_resolution_clock::now();

    std::thread producer([&]() {
        for (const auto& buffer : sent_buffers) {
            ipc.push(buffer);
        }
    });

    std::thread consumer([&]() {
        for (size_t i = 0; i < NUM_BUFFERS; ++i) {
            std::vector<float> received;
            ipc.pop_blocking(received);
        }
    });

    producer.join();
    consumer.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    double total_bytes = NUM_BUFFERS * BUFFER_SIZE;
    double throughput_mbps = (total_bytes / (duration_ms / 1000.0)) / (1024.0 * 1024.0);

    EXPECT_GT(throughput_mbps, MIN_THROUGHPUT_MBPS) << "Taxa de transferência: " << throughput_mbps << " MB/s";
}

// Teste 3: Fila vazia
TEST_F(IPCManagerTest, EmptyQueue) {
    auto result = ipc.pop();
    EXPECT_FALSE(result.has_value()) << "Fila vazia deve retornar std::nullopt";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}