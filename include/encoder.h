#ifndef ENCODER_H
#define ENCODER_H

#include <opus/opus.h>
#include <vector>

// Parâmetros de áudio
static constexpr int SAMPLE_RATE = 48000;
static constexpr int CHANNELS    = 2;
// FRAME_SIZE em samples por canal (20 ms em 48 kHz)
static constexpr int FRAME_SIZE  = SAMPLE_RATE / 1000 * 20;

class Encoder {
private:
    OpusEncoder* encoder_;

public:
    Encoder();
    ~Encoder();
    std::vector<unsigned char> encode(const std::vector<float>& input);
};

#endif // ENCODER_H
