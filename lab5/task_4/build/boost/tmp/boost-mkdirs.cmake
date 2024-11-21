# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/OC/lab5/task_4/build/boost/src/boost"
  "C:/OC/lab5/task_4/build/boost/src/boost-build"
  "C:/OC/lab5/task_4/build/boost"
  "C:/OC/lab5/task_4/build/boost/tmp"
  "C:/OC/lab5/task_4/build/boost/src/boost-stamp"
  "C:/OC/lab5/task_4/build/boost/src"
  "C:/OC/lab5/task_4/build/boost/src/boost-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/OC/lab5/task_4/build/boost/src/boost-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/OC/lab5/task_4/build/boost/src/boost-stamp${cfgdir}") # cfgdir has leading slash
endif()
