// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <functional>
#include <sched.h>
#include <stdexcept>

class Scheduler {
public:
    void scheduleTask(std::function<void()> task, int priority);
};

#endif // SCHEDULER_H
