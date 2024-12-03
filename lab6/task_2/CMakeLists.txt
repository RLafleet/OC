cmake_minimum_required(VERSION 3.10)

# Название проекта
project(task_2 LANGUAGES CXX)

# Установка стандартов C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем исполняемый файл
add_executable(task_2 task_2.cpp)

# Для Windows добавляем библиотеку synchronization
if(WIN32)
    target_link_libraries(task_2 synchronization)
endif()

# Подключение pthread для Linux
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(task_2 Threads::Threads)
endif()
