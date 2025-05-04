#ifndef SPEED_ADJUSTER_H
#define SPEED_ADJUSTER_H

#include "ipc_manager.h"   // para AudioBuffer
#include "audio_config.h"  // para constantes de áudio
#include <soundtouch/SoundTouch.h>
#include <vector>

class SpeedAdjuster {
private:
    soundtouch::SoundTouch soundtouch_;

public:
    SpeedAdjuster();
    void adjust(AudioBuffer& buffer, double buffer_ms);
};

#endif // SPEED_ADJUSTER_H
