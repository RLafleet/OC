cmake_minimum_required(VERSION 3.27)  # Установите минимальную версию CMake
project(MyBoostProject)                 # Название вашего проекта

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS *boost libraries here*)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(progname file1.cxx file2.cxx)
    target_link_libraries(progname ${Boost_LIBRARIES})
endif()

# Найдите необходимые компоненты Boost
find_package(Boost REQUIRED COMPONENTS system thread)

# Включите директории Boost
include_directories(${Boost_INCLUDE_DIRS})

# Добавьте исполняемый файл вашего проекта
add_executable(program_find task_4.cpp)  # Замените main.cpp на ваши исходные файлы

# Свяжите библиотеки Boost с вашим проектом
target_link_libraries(program_find ${Boost_LIBRARIES})