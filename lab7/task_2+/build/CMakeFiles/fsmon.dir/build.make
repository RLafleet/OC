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
CMAKE_SOURCE_DIR = /mnt/c/OC/lab7/task_2+

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/OC/lab7/task_2+/build

# Include any dependencies generated for this target.
include CMakeFiles/fsmon.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/fsmon.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/fsmon.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fsmon.dir/flags.make

CMakeFiles/fsmon.dir/task_2+.cpp.o: CMakeFiles/fsmon.dir/flags.make
CMakeFiles/fsmon.dir/task_2+.cpp.o: ../task_2+.cpp
CMakeFiles/fsmon.dir/task_2+.cpp.o: CMakeFiles/fsmon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/OC/lab7/task_2+/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fsmon.dir/task_2+.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fsmon.dir/task_2+.cpp.o -MF CMakeFiles/fsmon.dir/task_2+.cpp.o.d -o CMakeFiles/fsmon.dir/task_2+.cpp.o -c /mnt/c/OC/lab7/task_2+/task_2+.cpp

CMakeFiles/fsmon.dir/task_2+.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsmon.dir/task_2+.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/OC/lab7/task_2+/task_2+.cpp > CMakeFiles/fsmon.dir/task_2+.cpp.i

CMakeFiles/fsmon.dir/task_2+.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsmon.dir/task_2+.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/OC/lab7/task_2+/task_2+.cpp -o CMakeFiles/fsmon.dir/task_2+.cpp.s

# Object files for target fsmon
fsmon_OBJECTS = \
"CMakeFiles/fsmon.dir/task_2+.cpp.o"

# External object files for target fsmon
fsmon_EXTERNAL_OBJECTS =

fsmon: CMakeFiles/fsmon.dir/task_2+.cpp.o
fsmon: CMakeFiles/fsmon.dir/build.make
fsmon: CMakeFiles/fsmon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/OC/lab7/task_2+/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fsmon"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fsmon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fsmon.dir/build: fsmon
.PHONY : CMakeFiles/fsmon.dir/build

CMakeFiles/fsmon.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fsmon.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fsmon.dir/clean

CMakeFiles/fsmon.dir/depend:
	cd /mnt/c/OC/lab7/task_2+/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/OC/lab7/task_2+ /mnt/c/OC/lab7/task_2+ /mnt/c/OC/lab7/task_2+/build /mnt/c/OC/lab7/task_2+/build /mnt/c/OC/lab7/task_2+/build/CMakeFiles/fsmon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fsmon.dir/depend

