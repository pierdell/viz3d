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
include unsupported/test/CMakeFiles/autodiff_scalar_1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include unsupported/test/CMakeFiles/autodiff_scalar_1.dir/compiler_depend.make

# Include the progress variables for this target.
include unsupported/test/CMakeFiles/autodiff_scalar_1.dir/progress.make

# Include the compile flags for this target's objects.
include unsupported/test/CMakeFiles/autodiff_scalar_1.dir/flags.make

unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o: unsupported/test/CMakeFiles/autodiff_scalar_1.dir/flags.make
unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/test/autodiff_scalar.cpp
unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o: unsupported/test/CMakeFiles/autodiff_scalar_1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o -MF CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o.d -o CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o -c /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/test/autodiff_scalar.cpp

unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.i"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/test/autodiff_scalar.cpp > CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.i

unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.s"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/test/autodiff_scalar.cpp -o CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.s

# Object files for target autodiff_scalar_1
autodiff_scalar_1_OBJECTS = \
"CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o"

# External object files for target autodiff_scalar_1
autodiff_scalar_1_EXTERNAL_OBJECTS =

unsupported/test/autodiff_scalar_1: unsupported/test/CMakeFiles/autodiff_scalar_1.dir/autodiff_scalar.cpp.o
unsupported/test/autodiff_scalar_1: unsupported/test/CMakeFiles/autodiff_scalar_1.dir/build.make
unsupported/test/autodiff_scalar_1: unsupported/test/CMakeFiles/autodiff_scalar_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable autodiff_scalar_1"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/autodiff_scalar_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unsupported/test/CMakeFiles/autodiff_scalar_1.dir/build: unsupported/test/autodiff_scalar_1
.PHONY : unsupported/test/CMakeFiles/autodiff_scalar_1.dir/build

unsupported/test/CMakeFiles/autodiff_scalar_1.dir/clean:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test && $(CMAKE_COMMAND) -P CMakeFiles/autodiff_scalar_1.dir/cmake_clean.cmake
.PHONY : unsupported/test/CMakeFiles/autodiff_scalar_1.dir/clean

unsupported/test/CMakeFiles/autodiff_scalar_1.dir/depend:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/Eigen3/src/Eigen3 /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/test/CMakeFiles/autodiff_scalar_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unsupported/test/CMakeFiles/autodiff_scalar_1.dir/depend

