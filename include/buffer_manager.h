#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include "ipc_manager.h"

#define SAMPLE_RATE (48000) // Taxa de amostragem padrão (48 kHz)
#define CHANNELS (2)        // Número de canais (estéreo)

class BufferManager {
private:
    IPCManager& ipc_;       // Referência ao gerenciador de IPC
    double buffer_ms_;      // Tamanho do buffer em milissegundos

public:
    // Construtor que inicializa o BufferManager com um IPCManager
    BufferManager(IPCManager& ipc);

    // Adiciona um buffer ao IPCManager e atualiza o tamanho do buffer em ms
    void push(const AudioBuffer& buffer);

    // Remove um buffer do IPCManager e atualiza o tamanho do buffer em ms
    bool pop(AudioBuffer& buffer);

    // Retorna o tamanho atual do buffer em milissegundos
    double getBufferMs() const;
};

#endif // BUFFER_MANAGER_H