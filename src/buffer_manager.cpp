#include "buffer_manager.h"

BufferManager::BufferManager(IPCManager& ipc) : ipc_(ipc), buffer_ms_(0) {}

void BufferManager::push(const AudioBuffer& buffer) {
    buffer_ms_ += (buffer.size / (SAMPLE_RATE * CHANNELS)) * 1000.0;
    ipc_.push(buffer);
}

bool BufferManager::pop(AudioBuffer& buffer) {
    if (ipc_.pop(buffer)) {
        buffer_ms_ -= (buffer.size / (SAMPLE_RATE * CHANNELS)) * 1000.0;
        return true;
    }
    return false;
}

double BufferManager::getBufferMs() const {
    return buffer_ms_;
}