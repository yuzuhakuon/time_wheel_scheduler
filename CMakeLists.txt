# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(TimeWheelScheduler)

# 设置
set(CMAKE_CXX_STANDARD 11)
include_directories( ${PROJECT_SOURCE_DIR}/include )

# aux_source_directory(. Source_List)

# 动态库
set(TIMR_WHEEL_LIB_NAME "time_wheel")
add_library(${TIMR_WHEEL_LIB_NAME} SHARED time_wheel_scheduler.cpp time_wheel.cpp timer_task.cpp)
# 静态库
# set(TIMR_WHEEL_LIB_STATIC_NAME "time_wheel_static")
# add_library(${TIMR_WHEEL_LIB_STATIC_NAME} STATIC time_wheel_scheduler.cpp time_wheel.cpp timer_task.cpp)
# 编译源码生成目标
add_executable(${PROJECT_NAME} main.cpp)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TIMR_WHEEL_LIB_NAME} PUBLIC Threads::Threads)
# target_link_libraries(${TIMR_WHEEL_LIB_STATIC_NAME} PUBLIC Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${TIMR_WHEEL_LIB_NAME})
