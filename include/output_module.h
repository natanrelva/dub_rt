// output_module.h
#ifndef OUTPUT_MODULE_H
#define OUTPUT_MODULE_H

#include "buffer_manager.h"
#include <portaudio.h>

class OutputModule {
private:
    PaStream* output_stream_;
    BufferManager& local_buffer_; // PT-BR
    BufferManager& meets_buffer_; // EN

public:
    OutputModule(BufferManager& local_buffer, BufferManager& meets_buffer);
    ~OutputModule();
    void start();
    void stop();

private:
    static int outputCallback(const void* input, void* output, unsigned long frameCount,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags, void* userData);
};

#endif // OUTPUT_MODULE_H
