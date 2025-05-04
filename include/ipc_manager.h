// include/ipc_manager.h
#ifndef IPC_MANAGER_H
#define IPC_MANAGER_H

#include <queue>
#include <mutex>
#include <vector>

struct AudioBuffer {
    std::vector<float> data;
    size_t size;
    double timestamp;
    float speed_factor = 1.0f; // Adicionado para armazenar o fator de velocidade
};

class IPCManager {
private:
    std::queue<AudioBuffer> queue_;
    std::mutex mutex_;

public:
    void push(const AudioBuffer& buffer);
    bool pop(AudioBuffer& buffer);
};

#endif // IPC_MANAGER_H