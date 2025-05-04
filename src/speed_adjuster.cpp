// speed_adjuster.cpp
#include "speed_adjuster.h"

SpeedAdjuster::SpeedAdjuster() {
    soundtouch_.setSampleRate(SAMPLE_RATE);
    soundtouch_.setChannels(CHANNELS);
}

void SpeedAdjuster::adjust(AudioBuffer& buffer, double buffer_ms) {
    float speed = 1.0f;
    if (buffer_ms > 1000) speed = 1.1f;
    else if (buffer_ms < 200) speed = 0.9f;
    soundtouch_.setTempo(speed);
    buffer.speed_factor = speed;

    soundtouch_.putSamples(buffer.data.data(), buffer.size / CHANNELS);
    std::vector<float> adjusted;
    float temp[960 * CHANNELS];
    int samples;
    do {
        samples = soundtouch_.receiveSamples(temp, 960);
        adjusted.insert(adjusted.end(), temp, temp + samples * CHANNELS);
    } while (samples > 0);
    buffer.data = adjusted;
    buffer.size = adjusted.size();
}