// scheduler.cpp (ajustado para macOS)
#include "scheduler.h"
#ifdef __APPLE__
#include <pthread.h>
#else
#include <sched.h>
#endif

void Scheduler::scheduleTask(std::function<void()> task, int priority) {
    std::thread t([task, priority]() {
        #ifdef __APPLE__
        // macOS: usar QoS (exemplo simplificado)
        pthread_t thread = pthread_self();
        pthread_setname_np("SchedulerTask");
        // Configurar QoS (exemplo; ajustar conforme necessário)
        #else
        struct sched_param param;
        param.sched_priority = priority;
        if (sched_setscheduler(0, SCHED_FIFO, ¶m) != 0) {
            throw std::runtime_error("Failed to set scheduler priority");
        }
        #endif
        task();
    });
    t.detach();
}