#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

// #include <time_wheel_scheduler.h>
#include <time_wheel_scheduler.h>


std::chrono::system_clock::time_point t1;

int main()
{
    Task processor = []() {
        auto current = std::chrono::system_clock::now();
        long cost    = std::chrono::duration_cast<std::chrono::milliseconds>(current - t1).count();
        std::cout << "during time: " << cost << " ms" << std::endl;
    };

    // auto tws = std::make_shared<TimeWheelScheduler>(10)
    auto tws = CreateInstanceFactory2(20);
    tws->AppendTimeWheel(50, "w1");   // 秒级
    tws->AppendTimeWheel(60, "w2");    // 分钟
    tws->CreateTimerAfter(500, 600, processor);
    t1 = std::chrono::system_clock::now();
    tws->Start();

    std::this_thread::sleep_for(std::chrono::seconds(4));
    tws->Stop();
    std::cout << "stop instance" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return EXIT_SUCCESS;
}