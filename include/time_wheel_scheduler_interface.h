#ifndef INCLUDE_TIME_WHEEL_SCHEDULER_INTERFACE_H_
#define INCLUDE_TIME_WHEEL_SCHEDULER_INTERFACE_H_

#include <functional>
#include <memory>
#include <string>

#ifdef _WIN32
#    ifdef TIMEWHEEL_EXPORTS
#        define EXTERNAL_DLL extern "C" _declspec(dllexport)
#    else
#        define EXTERNAL_DLL extern "C" _declspec(dllimport)
#    endif
#else
#    define EXTERNAL_DLL extern "C"
#endif   // _WIN32



typedef std::function<void()> Task;


struct ITimeWheelScheduler
{
public:
    virtual void AppendTimeWheel(int num_units, const std::string& name)        = 0;
    virtual void CreateTimerEvery(int interval, const Task& func)               = 0;
    virtual void CreateTimerAfter(int delay_ms, int interval, const Task& func) = 0;
    virtual bool Start()                                                        = 0;
    virtual void Stop()                                                         = 0;
    virtual void Run()                                                          = 0;
};

EXTERNAL_DLL ITimeWheelScheduler* CreateInstanceFactory(long timer_step_ms);

static std::shared_ptr<ITimeWheelScheduler> CreateInstanceFactory2(long timer_step_ms)
{
    std::shared_ptr<ITimeWheelScheduler> tws(CreateInstanceFactory(timer_step_ms));
    return tws;
}

#endif   // !INCLUDE_TIME_WHEEL_SCHEDULER_INTERFACE_H_