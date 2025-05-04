#include "capture_module.h"
#include <stdexcept>
#include <iostream>

using namespace AudioConfig;

CaptureModule::CaptureModule(BufferManager& mic_buffer, BufferManager& loopback_buffer)
    : mic_buffer_(mic_buffer), loopback_buffer_(loopback_buffer),
      mic_stream_(nullptr), loopback_stream_(nullptr) {
    Pa_Initialize();
}

CaptureModule::~CaptureModule() {
    stop();
    Pa_Terminate();
}

void CaptureModule::start() {
    PaError err;

    err = Pa_OpenDefaultStream(&mic_stream_, CHANNELS, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER,
                               micCallback, &mic_buffer_);
    if (err != paNoError) {
        throw std::runtime_error("Failed to open mic stream: " + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_OpenDefaultStream(&loopback_stream_, CHANNELS, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER,
                               loopbackCallback, &loopback_buffer_);
    if (err != paNoError) {
        throw std::runtime_error("Failed to open loopback stream: " + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_StartStream(mic_stream_);
    if (err != paNoError) {
        throw std::runtime_error("Failed to start mic stream: " + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_StartStream(loopback_stream_);
    if (err != paNoError) {
        throw std::runtime_error("Failed to start loopback stream: " + std::string(Pa_GetErrorText(err)));
    }
}

void CaptureModule::stop() {
    if (mic_stream_) {
        Pa_StopStream(mic_stream_);
        Pa_CloseStream(mic_stream_);
        mic_stream_ = nullptr;
    }
    if (loopback_stream_) {
        Pa_StopStream(loopback_stream_);
        Pa_CloseStream(loopback_stream_);
        loopback_stream_ = nullptr;
    }
}

int CaptureModule::micCallback(const void* input, void* /*output*/, unsigned long frameCount,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags /*statusFlags*/, void* userData) {
    BufferManager* buffer = static_cast<BufferManager*>(userData);
    const float* in = static_cast<const float*>(input);

    AudioBuffer audio_buffer;
    audio_buffer.size = frameCount * CHANNELS;
    audio_buffer.data.assign(in, in + audio_buffer.size);
    audio_buffer.timestamp = timeInfo->inputBufferAdcTime;
    buffer->push(audio_buffer);

    return paContinue;
}

int CaptureModule::loopbackCallback(const void* input, void* /*output*/, unsigned long frameCount,
                                    const PaStreamCallbackTimeInfo* timeInfo,
                                    PaStreamCallbackFlags /*statusFlags*/, void* userData) {
    BufferManager* buffer = static_cast<BufferManager*>(userData);
    const float* in = static_cast<const float*>(input);

    AudioBuffer audio_buffer;
    audio_buffer.size = frameCount * CHANNELS;
    audio_buffer.data.assign(in, in + audio_buffer.size);
    audio_buffer.timestamp = timeInfo->inputBufferAdcTime;
    buffer->push(audio_buffer);

    return paContinue;
}
