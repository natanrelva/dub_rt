// include/speed_adjuster.h
#ifndef SPEED_ADJUSTER_H
#define SPEED_ADJUSTER_H

#include "ipc_manager.h"
#include "buffer_manager.h" // Adicionado para SAMPLE_RATE e CHANNELS
#include <SoundTouch.h>

class SpeedAdjuster {
private:
    soundtouch::SoundTouch soundtouch_;

public:
    SpeedAdjuster();
    void adjust(AudioBuffer& buffer, double buffer_ms);
};

#endif // SPEED_ADJUSTER_H