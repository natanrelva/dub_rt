#ifndef AUDIO_CONFIG_H
#define AUDIO_CONFIG_H

namespace AudioConfig {
    const int SAMPLE_RATE = 44100;    // Taxa de amostragem
    const int CHANNELS = 1;           // Canais de áudio (estéreo)
    const int FRAMES_PER_BUFFER = 1024;  // Número de quadros por buffer
}

#endif // AUDIO_CONFIG_H
