#include "time_wheel_scheduler.h"
#include <memory>


TimeWheelScheduler::TimeWheelScheduler(long timer_step_ms)
    : timer_step_ms_(timer_step_ms)
    , is_stop_(false)
    , max_delay_(timer_step_ms)
{}

void TimeWheelScheduler::AppendTimeWheel(int num_units, const std::string& name)
{
    if (time_wheels_.empty()) {   // 如果是第一个时间轮
        TimeWheelPtr time_wheel = std::make_shared<TimeWheel>(num_units, timer_step_ms_, name);
        time_wheels_.push_back(time_wheel);
    }
    else {
        TimeWheelPtr child_time_wheel = time_wheels_.back();
        TimeWheelPtr time_wheel =
            std::make_shared<TimeWheel>(num_units, child_time_wheel.get(), name);
        child_time_wheel->set_parent(time_wheel.get());
        time_wheels_.push_back(time_wheel);
    }
    max_delay_ = num_units * max_delay_;
}

void TimeWheelScheduler::CreateTimerEvery(int interval, const Task& func)
{
    std::lock_guard<std::mutex> lock(mut_);

    auto         bottom_layer = GetBottomTimeWheel();
    int          delay_ms     = bottom_layer->current_index() * bottom_layer->interval();
    TimerTaskPtr task         = std::make_shared<TimerTask>(delay_ms, interval, func);
    bottom_layer->AddTask(task);
}

void TimeWheelScheduler::CreateTimerAfter(int delay_ms, int interval, const Task& func)
{
    std::lock_guard<std::mutex> lock(mut_);

    auto bottom_layer = GetBottomTimeWheel();
    auto top_layer    = GetTopTimeWheel();

    delay_ms += bottom_layer->current_index() * bottom_layer->interval();
    if (delay_ms > top_layer->num_units() * top_layer->interval()) {
        throw std::runtime_error("Delay is too high");
    }

    TimerTaskPtr task = std::make_shared<TimerTask>(delay_ms, interval, func);
    top_layer->AddTask(task);
}

bool TimeWheelScheduler::Start()
{
    if (time_wheels_.empty()) { return false; }
    thread_ = std::thread(std::bind(&TimeWheelScheduler::Run, this));

    return true;
}

void TimeWheelScheduler::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mut_);
        is_stop_ = true;
    }

    thread_.join();
}

void TimeWheelScheduler::Run()
{
    start_time_ = std::chrono::system_clock::now();
    while (true) {
        std::chrono::system_clock::time_point awake_time =
            start_time_ + std::chrono::milliseconds(timer_step_ms_);
        if (is_stop_) { break; }

        {
            std::lock_guard<std::mutex> lock(mut_);

            auto                    bottom_layer = GetBottomTimeWheel();
            std::list<TimerTaskPtr> slot = std::move(bottom_layer->GetAndClearCurrentSlot());
            for (const TimerTaskPtr& task : slot) {
                std::thread task_thread = std::thread(std::bind(&TimerTask::Run, task));
                task_thread.detach();

                if (task->repeated()) {
                    auto top_time_wheel = GetTopTimeWheel();

                    int delay_ms =
                        task->interval() + bottom_layer->current_index() * bottom_layer->interval();
                    task->set_after(delay_ms);
                    top_time_wheel->AddTask(task);
                }
            }
            bottom_layer->Increase();
        }

        std::this_thread::sleep_until(awake_time);
        start_time_ = awake_time;
    }
}

TimeWheelPtr TimeWheelScheduler::GetTopTimeWheel() const
{
    if (time_wheels_.empty()) { return TimeWheelPtr(); }
    return time_wheels_.back();
}

TimeWheelPtr TimeWheelScheduler::GetBottomTimeWheel() const
{
    if (time_wheels_.empty()) { return TimeWheelPtr(); }
    return time_wheels_.front();
}


ITimeWheelScheduler* CreateInstanceFactory(long timer_step_ms)
{
    auto tws = new TimeWheelScheduler(timer_step_ms);
    return tws;
}
