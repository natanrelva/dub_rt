#ifndef IPC_MANAGER_H
#define IPC_MANAGER_H

#include <queue>
#include <mutex>
#include <vector>

// Definição única de AudioBuffer para todo o projeto
struct AudioBuffer {
    std::vector<float> data;
    size_t size;
    double timestamp;
    float speed_factor = 1.0f;
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
