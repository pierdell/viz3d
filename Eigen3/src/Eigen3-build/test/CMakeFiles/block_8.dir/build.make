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
include test/CMakeFiles/block_8.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/block_8.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/block_8.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/block_8.dir/flags.make

test/CMakeFiles/block_8.dir/block.cpp.o: test/CMakeFiles/block_8.dir/flags.make
test/CMakeFiles/block_8.dir/block.cpp.o: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/block.cpp
test/CMakeFiles/block_8.dir/block.cpp.o: test/CMakeFiles/block_8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/block_8.dir/block.cpp.o"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/block_8.dir/block.cpp.o -MF CMakeFiles/block_8.dir/block.cpp.o.d -o CMakeFiles/block_8.dir/block.cpp.o -c /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/block.cpp

test/CMakeFiles/block_8.dir/block.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/block_8.dir/block.cpp.i"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/block.cpp > CMakeFiles/block_8.dir/block.cpp.i

test/CMakeFiles/block_8.dir/block.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/block_8.dir/block.cpp.s"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/block.cpp -o CMakeFiles/block_8.dir/block.cpp.s

# Object files for target block_8
block_8_OBJECTS = \
"CMakeFiles/block_8.dir/block.cpp.o"

# External object files for target block_8
block_8_EXTERNAL_OBJECTS =

test/block_8: test/CMakeFiles/block_8.dir/block.cpp.o
test/block_8: test/CMakeFiles/block_8.dir/build.make
test/block_8: test/CMakeFiles/block_8.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable block_8"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/block_8.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/block_8.dir/build: test/block_8
.PHONY : test/CMakeFiles/block_8.dir/build

test/CMakeFiles/block_8.dir/clean:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -P CMakeFiles/block_8.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/block_8.dir/clean

test/CMakeFiles/block_8.dir/depend:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/Eigen3/src/Eigen3 /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test/CMakeFiles/block_8.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/block_8.dir/depend

