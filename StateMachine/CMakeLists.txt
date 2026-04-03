cmake_minimum_required(VERSION 3.10)

# 定义工程名称
project(StateMachine)

# 添加可执行文件目标
add_executable(StateMachine main.cpp)

# 指定该目标需要 C++17 标准（由于使用了 std::variant 和 inline 变量）
target_compile_features(StateMachine PRIVATE cxx_std_17)

# 指定头文件搜索路径，确保 main.cpp 可以直接 include "Door.h"
target_include_directories(StateMachine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)