// Main.cpp
#include "IPCManager.h"
#include <iostream>
#include <thread>

int main() {
    IPCManager<int> ipc;

    std::thread sender([&ipc]() {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Enviando: " << i << std::endl;
            ipc.push(i);
        }
    });

    std::thread receiver([&ipc]() {
        int value;
        for (int i = 0; i < 5; ++i) {
            ipc.pop_blocking(value);
            std::cout << "Recebido: " << value << std::endl;
        }
    });

    sender.join();
    receiver.join();

    return 0;
}
