#include "time_wheel.h"

TimeWheel::TimeWheel(int num_units, TimeWheel* child, const std::string& name)
    : num_units_(num_units)
    , interval_(1)
    , name_(name)
    , current_index_(0)
    , slots_(num_units)
    , child_(child)
    , parent_(nullptr)
{
    if (child_) {
        child_->set_parent(this);
        interval_ = child_->num_units() * child_->interval();
    }
}

TimeWheel::TimeWheel(int num_units, int interval, const std::string& name)
    : num_units_(num_units)
    , interval_(interval)
    , name_(name)
    , current_index_(0)
    , slots_(num_units)
    , child_(nullptr)
    , parent_(nullptr)
{}

void TimeWheel::Increase()
{
    current_index_ = (current_index_ + 1) % num_units_;
    if (current_index_ == 0) {
        if (parent_) {
            parent_->Increase();
            auto slot = std::move(parent_->GetAndClearCurrentSlot());
            for (auto task : slot) { AddTask(task); }
        }
    }
}

std::list<TimerTaskPtr> TimeWheel::GetAndClearCurrentSlot()
{
    std::list<TimerTaskPtr> slot;
    slot = std::move(slots_[current_index_]);
    return slot;
}


void TimeWheel::AddTask(TimerTaskPtr task)
{
    int inc_index = task->after() / interval_;
    task->set_after(task->after() % interval_);

    if (child_) {
        if (inc_index == 0) { this->child_->AddTask(task); }
        else {
            slots_[current_index_ + inc_index].push_back(task);
        }
    }
    else {
        slots_[inc_index].push_back(task);
    }
}