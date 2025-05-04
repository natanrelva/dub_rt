#ifndef CAPTURE_MODULE_H
#define CAPTURE_MODULE_H

#include "buffer_manager.h"
#include "audio_config.h"
#include <portaudio.h>

class CaptureModule {
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

    BufferManager& mic_buffer_;
    BufferManager& loopback_buffer_;
    PaStream* mic_stream_;
    PaStream* loopback_stream_;
};

#endif // CAPTURE_MODULE_H
