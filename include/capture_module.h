// capture_module.h
#ifndef CAPTURE_MODULE_H
#define CAPTURE_MODULE_H

#include "buffer_manager.h"
#include <portaudio.h>
#include <cstddef>

// Parâmetros de áudio — o mesmo que o Encoder
static constexpr int CAPTURE_SAMPLE_RATE = 48000;
static constexpr int CAPTURE_CHANNELS    = 2;
// FRAME_SIZE em samples por canal: 20 ms → 48000 Hz * 0.02 = 960
static constexpr int FRAME_SIZE  = CAPTURE_SAMPLE_RATE / 1000 * 20;

class CaptureModule {
private:
    PaStream* mic_stream_;
    PaStream* loopback_stream_;
    BufferManager& mic_buffer_;
    BufferManager& loopback_buffer_;

public:
    CaptureModule(BufferManager& mic_buffer, BufferManager& loopback_buffer);
    ~CaptureModule();
    void start();
    void stop();

private:
    static int micCallback(const void* input, void* output, unsigned long frameCount,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags, void* userData);
    static int loopbackCallback(const void* input, void* output, unsigned long frameCount,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags, void* userData);
};

#endif // CAPTURE_MODULE_H
