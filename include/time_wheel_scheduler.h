#ifndef INCLUDE_TIME_WHEEL_SCHEDULER_H_
#define INCLUDE_TIME_WHEEL_SCHEDULER_H_
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

#include "time_wheel.h"
#include "time_wheel_scheduler_interface.h"


typedef std::function<void()> Task;

class TimeWheelScheduler : public ITimeWheelScheduler
{
public:
    TimeWheelScheduler(long timer_step_ms = 20);

    void AppendTimeWheel(int num_units, const std::string& name);

    void CreateTimerEvery(int interval, const Task& func);

    void CreateTimerAfter(int delay_ms, int interval, const Task& func);


    bool Start();

    void Stop();

    void Run();

    TimeWheelPtr GetTopTimeWheel() const;
    TimeWheelPtr GetBottomTimeWheel() const;

private:
    int                                     timer_step_ms_;
    int                                     max_delay_;
    bool                                    is_stop_;
    std::mutex                              mut_;
    std::thread                             thread_;
    std::chrono::system_clock::time_point   start_time_;
    std::vector<std::shared_ptr<TimeWheel>> time_wheels_;
};

#endif   // !INCLUDE_TIME_WHEEL_SCHEDULER_H_