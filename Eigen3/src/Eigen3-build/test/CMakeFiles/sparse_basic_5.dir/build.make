# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_SOURCE_DIR = /home/pdell/dev/viz3d/Eigen3/src/Eigen3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build

# Include any dependencies generated for this target.
include test/CMakeFiles/sparse_basic_5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/sparse_basic_5.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/sparse_basic_5.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/sparse_basic_5.dir/flags.make

test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o: test/CMakeFiles/sparse_basic_5.dir/flags.make
test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/sparse_basic.cpp
test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o: test/CMakeFiles/sparse_basic_5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o -MF CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o.d -o CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o -c /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/sparse_basic.cpp

test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.i"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/sparse_basic.cpp > CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.i

test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.s"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/sparse_basic.cpp -o CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.s

# Object files for target sparse_basic_5
sparse_basic_5_OBJECTS = \
"CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o"

# External object files for target sparse_basic_5
sparse_basic_5_EXTERNAL_OBJECTS =

test/sparse_basic_5: test/CMakeFiles/sparse_basic_5.dir/sparse_basic.cpp.o
test/sparse_basic_5: test/CMakeFiles/sparse_basic_5.dir/build.make
test/sparse_basic_5: test/CMakeFiles/sparse_basic_5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sparse_basic_5"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sparse_basic_5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/sparse_basic_5.dir/build: test/sparse_basic_5
.PHONY : test/CMakeFiles/sparse_basic_5.dir/build

test/CMakeFiles/sparse_basic_5.dir/clean:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -P CMakeFiles/sparse_basic_5.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/sparse_basic_5.dir/clean

test/CMakeFiles/sparse_basic_5.dir/depend:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/Eigen3/src/Eigen3 /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test/CMakeFiles/sparse_basic_5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/sparse_basic_5.dir/depend

