# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/OC/lab2/task_3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/OC/lab2/task_3/build

# Include any dependencies generated for this target.
include CMakeFiles/zombie-linux.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/zombie-linux.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/zombie-linux.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zombie-linux.dir/flags.make

CMakeFiles/zombie-linux.dir/main.cpp.o: CMakeFiles/zombie-linux.dir/flags.make
CMakeFiles/zombie-linux.dir/main.cpp.o: ../main.cpp
CMakeFiles/zombie-linux.dir/main.cpp.o: CMakeFiles/zombie-linux.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/OC/lab2/task_3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zombie-linux.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/zombie-linux.dir/main.cpp.o -MF CMakeFiles/zombie-linux.dir/main.cpp.o.d -o CMakeFiles/zombie-linux.dir/main.cpp.o -c /mnt/c/OC/lab2/task_3/main.cpp

CMakeFiles/zombie-linux.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zombie-linux.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/OC/lab2/task_3/main.cpp > CMakeFiles/zombie-linux.dir/main.cpp.i

CMakeFiles/zombie-linux.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zombie-linux.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/OC/lab2/task_3/main.cpp -o CMakeFiles/zombie-linux.dir/main.cpp.s

# Object files for target zombie-linux
zombie__linux_OBJECTS = \
"CMakeFiles/zombie-linux.dir/main.cpp.o"

# External object files for target zombie-linux
zombie__linux_EXTERNAL_OBJECTS =

zombie-linux: CMakeFiles/zombie-linux.dir/main.cpp.o
zombie-linux: CMakeFiles/zombie-linux.dir/build.make
zombie-linux: CMakeFiles/zombie-linux.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/OC/lab2/task_3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable zombie-linux"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zombie-linux.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zombie-linux.dir/build: zombie-linux
.PHONY : CMakeFiles/zombie-linux.dir/build

CMakeFiles/zombie-linux.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zombie-linux.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zombie-linux.dir/clean

CMakeFiles/zombie-linux.dir/depend:
	cd /mnt/c/OC/lab2/task_3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/OC/lab2/task_3 /mnt/c/OC/lab2/task_3 /mnt/c/OC/lab2/task_3/build /mnt/c/OC/lab2/task_3/build /mnt/c/OC/lab2/task_3/build/CMakeFiles/zombie-linux.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zombie-linux.dir/depend
