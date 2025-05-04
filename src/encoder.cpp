#include "encoder.h"
#include <stdexcept>

Encoder::Encoder() {
    int error;
    encoder_ = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK) {
        throw std::runtime_error("Failed to create Opus encoder");
    }
    opus_encoder_ctl(encoder_, OPUS_SET_BITRATE(24000)); // 24 kbps
}

Encoder::~Encoder() {
    opus_encoder_destroy(encoder_);
}

std::vector<unsigned char> Encoder::encode(const std::vector<float>& input) {
    std::vector<unsigned char> output(4000); // Tamanho máximo para quadro de 20ms
    int bytes = opus_encode_float(encoder_, input.data(), FRAME_SIZE, output.data(), output.size());
    if (bytes < 0) {
        throw std::runtime_error("Opus encode failed");
    }
    output.resize(bytes);
    return output;
}