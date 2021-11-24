#ifndef INCLUDE_TIME_WHEEL_H_
#define INCLUDE_TIME_WHEEL_H_

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <vector>

#include "timer_task.h"

class TimeWheel
{
public:
    TimeWheel(int num_units, TimeWheel* child, const std::string& name);

    TimeWheel(int num_units, int interval, const std::string& name);

    void Increase();

    std::list<TimerTaskPtr> GetAndClearCurrentSlot();

    void AddTask(TimerTaskPtr task);


    int         num_units() const { return num_units_; }
    int         interval() const { return interval_; }
    int         current_index() const { return current_index_; }
    std::string name() const { return name_; }

    TimeWheel* child() const { return child_; }
    TimeWheel* parent() const { return parent_; }
    void       set_parent(TimeWheel* parent) { parent_ = parent; }

private:
    int                                  num_units_;
    int                                  current_index_;
    int                                  interval_;
    std::string                          name_;
    TimeWheel*                           child_;
    TimeWheel*                           parent_;
    std::vector<std::list<TimerTaskPtr>> slots_;
};

using TimeWheelPtr = std::shared_ptr<TimeWheel>;

#endif   // !INCLUDE_TIME_WHEEL_H_
