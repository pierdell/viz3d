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
include test/CMakeFiles/mpl2only.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/mpl2only.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/mpl2only.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/mpl2only.dir/flags.make

test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o: test/CMakeFiles/mpl2only.dir/flags.make
test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/mpl2only.cpp
test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o: test/CMakeFiles/mpl2only.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o -MF CMakeFiles/mpl2only.dir/mpl2only.cpp.o.d -o CMakeFiles/mpl2only.dir/mpl2only.cpp.o -c /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/mpl2only.cpp

test/CMakeFiles/mpl2only.dir/mpl2only.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mpl2only.dir/mpl2only.cpp.i"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/mpl2only.cpp > CMakeFiles/mpl2only.dir/mpl2only.cpp.i

test/CMakeFiles/mpl2only.dir/mpl2only.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mpl2only.dir/mpl2only.cpp.s"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test/mpl2only.cpp -o CMakeFiles/mpl2only.dir/mpl2only.cpp.s

# Object files for target mpl2only
mpl2only_OBJECTS = \
"CMakeFiles/mpl2only.dir/mpl2only.cpp.o"

# External object files for target mpl2only
mpl2only_EXTERNAL_OBJECTS =

test/mpl2only: test/CMakeFiles/mpl2only.dir/mpl2only.cpp.o
test/mpl2only: test/CMakeFiles/mpl2only.dir/build.make
test/mpl2only: test/CMakeFiles/mpl2only.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mpl2only"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mpl2only.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/mpl2only.dir/build: test/mpl2only
.PHONY : test/CMakeFiles/mpl2only.dir/build

test/CMakeFiles/mpl2only.dir/clean:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test && $(CMAKE_COMMAND) -P CMakeFiles/mpl2only.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/mpl2only.dir/clean

test/CMakeFiles/mpl2only.dir/depend:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/Eigen3/src/Eigen3 /home/pdell/dev/viz3d/Eigen3/src/Eigen3/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/test/CMakeFiles/mpl2only.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/mpl2only.dir/depend

