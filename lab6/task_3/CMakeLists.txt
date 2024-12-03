cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(FileSorter LANGUAGES CXX)

# Включаем стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sorter
    task_3.cpp
    Generator.cpp
    Sorter.cpp
    Utils.cpp
)
