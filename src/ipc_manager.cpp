// ipc_manager.cpp
#include "ipc_manager.h"

void IPCManager::push(const AudioBuffer& buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(buffer);
}

bool IPCManager::pop(AudioBuffer& buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!queue_.empty()) {
        buffer = queue_.front();
        queue_.pop();
        return true;
    }
    return false;
}