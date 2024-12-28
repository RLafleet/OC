cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -pedantic -Wconversion -Werror)
endif()

set(SOURCES
    tasks.cpp
)

set(HEADERS
    Server.h
    Socket.h
)

add_executable(calc ${SOURCES} ${HEADERS})

target_link_libraries(calc PRIVATE pthread)
