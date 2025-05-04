#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include "ipc_manager.h"
#include "audio_config.h"

class BufferManager {
public:
    // Construtor precisa ser declarado aqui
    explicit BufferManager(IPCManager& ipc);

    void push(const AudioBuffer& buffer);
    bool pop(AudioBuffer& buffer);

    double getBufferMs() const noexcept;

private:
    IPCManager& ipc_;
    double buffer_ms_ = 0.0;
};

#endif // BUFFER_MANAGER_H
