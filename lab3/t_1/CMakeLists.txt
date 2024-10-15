cmake_minimum_required(VERSION 3.10)  # Укажите минимальную версию CMake
project(MySignalHandlerProject)        # Название вашего проекта

set(CMAKE_CXX_STANDARD 20)             # Установка стандарта C++
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Требовать стандарт C++

# Добавление исполняемого файла
add_executable(MySignalHandler main.cpp 
	SignalHanderLinux.cpp
    SignalHander.cpp)