#ifndef INCLUDE_TIMER_TASK_H_
#define INCLUDE_TIMER_TASK_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>


typedef std::function<void()> Task;

class TimerTask
{
public:
    TimerTask(int after, int interval, const Task& task)
        : after_(after)
        , interval_(interval)
        , repeated_(interval > 0)
        , task_(task)
    {}

    void Run()
    {
        if (task_) {
            task_();
            count_++;
        }
    }

    long id() const { return id_; }

    long count() const { return count_; }

    int interval() const { return interval_; }

    bool repeated() const { return repeated_; }

    int  after() { return after_; }
    void set_after(int delay_ms) { after_ = delay_ms; }

private:
    long id_;
    int  after_;
    int  interval_;
    int  count_;
    bool repeated_;
    Task task_;

    // std::unordered_map<std::string, int> position_;
};

using TimerTaskPtr = std::shared_ptr<TimerTask>;

#endif   // !INCLUDE_TIMER_TASK_H_
