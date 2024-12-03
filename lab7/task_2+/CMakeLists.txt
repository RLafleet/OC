cmake_minimum_required(VERSION 3.10)
project(fsmon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(fsmon 
	task_2+.cpp,
	InotifyHandle.cpp
)
