#include "output_module.h"
#include <stdexcept>

#ifndef FRAME_SIZE
#define FRAME_SIZE 256
#endif

OutputModule::OutputModule(BufferManager& local_buffer, BufferManager& meets_buffer)
    : local_buffer_(local_buffer), meets_buffer_(meets_buffer), output_stream_(nullptr) {
    Pa_Initialize();
}

OutputModule::~OutputModule() {
    stop();
    Pa_Terminate();
}

void OutputModule::start() {
    PaError err = Pa_OpenDefaultStream(&output_stream_, 0, CHANNELS, paFloat32, SAMPLE_RATE, FRAME_SIZE,
                                       outputCallback, this);
    if (err != paNoError) {
        throw std::runtime_error("Failed to open output stream: " + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_StartStream(output_stream_);
    if (err != paNoError) {
        throw std::runtime_error("Failed to start output stream: " + std::string(Pa_GetErrorText(err)));
    }
}

void OutputModule::stop() {
    if (output_stream_) {
        Pa_StopStream(output_stream_);
        Pa_CloseStream(output_stream_);
        output_stream_ = nullptr;
    }
}

int OutputModule::outputCallback(const void* input, void* output, unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags, void* userData) {
    OutputModule* module = static_cast<OutputModule*>(userData);
    float* out = static_cast<float*>(output);

    AudioBuffer local_data;
    if (module->local_buffer_.pop(local_data)) {
        for (size_t i = 0; i < local_data.size && i < frameCount * CHANNELS; ++i) {
            out[i] = local_data.data[i];
        }
    } else {
        std::fill(out, out + frameCount * CHANNELS, 0.0f);
    }

    AudioBuffer meets_data;
    if (module->meets_buffer_.pop(meets_data)) {
        // Simular envio ao driver virtual (futuramente PulseAudio)
        // Exemplo: pactl load-module module-null-sink sink_name=VirtualMic
    }

    return paContinue;
}