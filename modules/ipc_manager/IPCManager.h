#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class IPCManager {
public:
    IPCManager() = default;
    ~IPCManager() = default;

    // Envia um item para a fila
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        cond_var_.notify_one();
    }

    // Recebe um item da fila, retorna std::optional<T> (vazio se a fila estiver vazia)
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        T item = queue_.front();
        queue_.pop();
        return item;
    }

    // Recebe um item com espera bloqueante
    bool pop_blocking(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !queue_.empty(); });
        item = queue_.front();
        queue_.pop();
        return true;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};