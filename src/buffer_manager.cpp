#include "buffer_manager.h"

using namespace AudioConfig;

// Implementação do construtor
BufferManager::BufferManager(IPCManager& ipc)
    : ipc_(ipc), buffer_ms_(0.0)
{}

// push: envia para IPC e acumula ms
void BufferManager::push(const AudioBuffer& buffer) {
    ipc_.push(buffer);
    buffer_ms_ += (buffer.size / static_cast<double>(SAMPLE_RATE * CHANNELS)) * 1000.0;
}

// pop: retira da IPC e desconta ms
bool BufferManager::pop(AudioBuffer& buffer) {
    if (!ipc_.pop(buffer)) return false;
    buffer_ms_ -= (buffer.size / static_cast<double>(SAMPLE_RATE * CHANNELS)) * 1000.0;
    if (buffer_ms_ < 0.0) buffer_ms_ = 0.0;
    return true;
}

// getter de ms
double BufferManager::getBufferMs() const noexcept {
    return buffer_ms_;
}
