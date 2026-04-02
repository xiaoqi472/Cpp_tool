cmake_minimum_required(VERSION 3.10)
project(MjpegStreamer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib
)

add_executable(${PROJECT_NAME}
    main.cpp
    MjpegStreamer.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
)
