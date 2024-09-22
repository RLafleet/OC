cmake_minimum_required(VERSION 3.10)

project(PrimeChecker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(PrimeChecker main.cpp PrimeChecker.cpp)

install(TARGETS PrimeChecker DESTINATION bin)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
endif()