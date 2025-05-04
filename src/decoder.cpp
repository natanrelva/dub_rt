#include "decoder.h"
#include <stdexcept>

Decoder::Decoder() {
    int error;
    decoder_ = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error);
    if (error != OPUS_OK) {
        throw std::runtime_error("Failed to create Opus decoder");
    }
}

Decoder::~Decoder() {
    opus_decoder_destroy(decoder_);
}

std::vector<float> Decoder::decode(const std::vector<unsigned char>& input) {
    std::vector<float> output(FRAME_SIZE * CHANNELS);
    int samples = opus_decode_float(decoder_, input.data(), input.size(), output.data(), FRAME_SIZE, 0);
    if (samples < 0) {
        throw std::runtime_error("Opus decode failed");
    }
    output.resize(samples * CHANNELS);
    return output;
}