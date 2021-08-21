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
include doc/snippets/CMakeFiles/compile_IOFormat.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include doc/snippets/CMakeFiles/compile_IOFormat.dir/compiler_depend.make

# Include the progress variables for this target.
include doc/snippets/CMakeFiles/compile_IOFormat.dir/progress.make

# Include the compile flags for this target's objects.
include doc/snippets/CMakeFiles/compile_IOFormat.dir/flags.make

doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o: doc/snippets/CMakeFiles/compile_IOFormat.dir/flags.make
doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o: doc/snippets/compile_IOFormat.cpp
doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/doc/snippets/IOFormat.cpp
doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o: doc/snippets/CMakeFiles/compile_IOFormat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o -MF CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o.d -o CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o -c /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets/compile_IOFormat.cpp

doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.i"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets/compile_IOFormat.cpp > CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.i

doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.s"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets/compile_IOFormat.cpp -o CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.s

# Object files for target compile_IOFormat
compile_IOFormat_OBJECTS = \
"CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o"

# External object files for target compile_IOFormat
compile_IOFormat_EXTERNAL_OBJECTS =

doc/snippets/compile_IOFormat: doc/snippets/CMakeFiles/compile_IOFormat.dir/compile_IOFormat.cpp.o
doc/snippets/compile_IOFormat: doc/snippets/CMakeFiles/compile_IOFormat.dir/build.make
doc/snippets/compile_IOFormat: doc/snippets/CMakeFiles/compile_IOFormat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable compile_IOFormat"
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compile_IOFormat.dir/link.txt --verbose=$(VERBOSE)
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && ./compile_IOFormat >/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets/IOFormat.out

# Rule to build all files generated by this target.
doc/snippets/CMakeFiles/compile_IOFormat.dir/build: doc/snippets/compile_IOFormat
.PHONY : doc/snippets/CMakeFiles/compile_IOFormat.dir/build

doc/snippets/CMakeFiles/compile_IOFormat.dir/clean:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets && $(CMAKE_COMMAND) -P CMakeFiles/compile_IOFormat.dir/cmake_clean.cmake
.PHONY : doc/snippets/CMakeFiles/compile_IOFormat.dir/clean

doc/snippets/CMakeFiles/compile_IOFormat.dir/depend:
	cd /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/Eigen3/src/Eigen3 /home/pdell/dev/viz3d/Eigen3/src/Eigen3/doc/snippets /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets /home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/doc/snippets/CMakeFiles/compile_IOFormat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/snippets/CMakeFiles/compile_IOFormat.dir/depend

