#include "speed_adjuster.h"
#include "audio_config.h"

SpeedAdjuster::SpeedAdjuster() {
    soundtouch_.setSampleRate(AudioConfig::SAMPLE_RATE);
    soundtouch_.setChannels(AudioConfig::CHANNELS);
}

void SpeedAdjuster::adjust(AudioBuffer& buffer, double buffer_ms) {
    float speed = 1.0f;
    if (buffer_ms > 1000.0) speed = 1.1f;
    else if (buffer_ms < 200.0) speed = 0.9f;

    soundtouch_.setTempo(speed);
    buffer.speed_factor = speed;

    // Envia amostras para processamento
    soundtouch_.putSamples(buffer.data.data(), buffer.size / AudioConfig::CHANNELS);

    // Recebe amostras ajustadas
    std::vector<float> adjusted;
    adjusted.reserve(buffer.data.size());
    std::vector<float> temp(AudioConfig::FRAMES_PER_BUFFER * AudioConfig::CHANNELS);  // Corrigir acesso à constante

    int num;
    do {
        num = soundtouch_.receiveSamples(temp.data(), AudioConfig::FRAMES_PER_BUFFER);  // Corrigir acesso à constante
        adjusted.insert(adjusted.end(), temp.begin(), temp.begin() + num * AudioConfig::CHANNELS);
    } while (num > 0);

    buffer.data = std::move(adjusted);
    buffer.size = buffer.data.size();
}
