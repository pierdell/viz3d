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
CMAKE_SOURCE_DIR = /home/pdell/dev/viz3d/glad/src/glad

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pdell/dev/viz3d/glad/src/glad-build

# Utility rule file for glad-generate-files.

# Include any custom commands dependencies for this target.
include CMakeFiles/glad-generate-files.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/glad-generate-files.dir/progress.make

CMakeFiles/glad-generate-files: src/glad.c
CMakeFiles/glad-generate-files: include/glad/glad.h

src/glad.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/glad/src/glad-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating GLAD"
	cd /home/pdell/dev/viz3d/glad/src/glad && /usr/bin/python3.6 -m glad --profile=compatibility --out-path=/home/pdell/dev/viz3d/glad/src/glad-build --api= --generator=c --extensions= --spec=gl

include/glad/glad.h: src/glad.c
	@$(CMAKE_COMMAND) -E touch_nocreate include/glad/glad.h

glad-generate-files: CMakeFiles/glad-generate-files
glad-generate-files: include/glad/glad.h
glad-generate-files: src/glad.c
glad-generate-files: CMakeFiles/glad-generate-files.dir/build.make
.PHONY : glad-generate-files

# Rule to build all files generated by this target.
CMakeFiles/glad-generate-files.dir/build: glad-generate-files
.PHONY : CMakeFiles/glad-generate-files.dir/build

CMakeFiles/glad-generate-files.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glad-generate-files.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glad-generate-files.dir/clean

CMakeFiles/glad-generate-files.dir/depend:
	cd /home/pdell/dev/viz3d/glad/src/glad-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/glad/src/glad /home/pdell/dev/viz3d/glad/src/glad /home/pdell/dev/viz3d/glad/src/glad-build /home/pdell/dev/viz3d/glad/src/glad-build /home/pdell/dev/viz3d/glad/src/glad-build/CMakeFiles/glad-generate-files.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glad-generate-files.dir/depend

