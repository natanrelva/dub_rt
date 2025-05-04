#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include <vector>

struct AudioBuffer {
    std::vector<int16_t> data;  // Amostras de áudio de 16 bits
    size_t size;                // Tamanho total do buffer (em amostras)

    // Construtores
    AudioBuffer() : size(0) {}
    AudioBuffer(const std::vector<int16_t>& data) : data(data), size(data.size()) {}
};

#endif // AUDIO_BUFFER_H
