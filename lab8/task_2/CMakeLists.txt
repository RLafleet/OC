cmake_minimum_required(VERSION 3.10)
project(Snowfall)

set(CMAKE_CXX_STANDARD 17)

find_package(X11 REQUIRED)

if (X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    message(STATUS "X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES: ${X11_LIBRARIES}")
else()
    message(FATAL_ERROR "X11 library not found")
endif()

add_executable(Snowfall main.cpp Display.cpp EventHandler.cpp Snowflake.cpp)
target_link_libraries(Snowfall ${X11_LIBRARIES})
