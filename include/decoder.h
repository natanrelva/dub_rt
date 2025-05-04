#ifndef DECODER_H
#define DECODER_H

#include <opus/opus.h>
#include <vector>

#define SAMPLE_RATE (48000)
#define CHANNELS (2)
#define FRAME_SIZE (960) // 20ms a 48kHz

class Decoder {
private:
    OpusDecoder* decoder_;

public:
    Decoder();
    ~Decoder();
    std::vector<float> decode(const std::vector<unsigned char>& input);
};

#endif // DECODER_H