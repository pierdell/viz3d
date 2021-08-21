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
CMAKE_SOURCE_DIR = /home/pdell/dev/viz3d/external

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pdell/dev/viz3d

# Utility rule file for glog.

# Include any custom commands dependencies for this target.
include CMakeFiles/glog.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/glog.dir/progress.make

CMakeFiles/glog: CMakeFiles/glog-complete

CMakeFiles/glog-complete: glog/src/glog-stamp/glog-install
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-mkdir
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-download
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-update
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-patch
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-configure
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-build
CMakeFiles/glog-complete: glog/src/glog-stamp/glog-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'glog'"
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/CMakeFiles
	/usr/bin/cmake -E touch /home/pdell/dev/viz3d/CMakeFiles/glog-complete
	/usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-done

glog/src/glog-stamp/glog-build: glog/src/glog-stamp/glog-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'glog'"
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake --build /home/pdell/dev/viz3d/glog/src/glog-build --config Release --target install
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake -E copy_directory /home/pdell/dev/viz3d/install/Release/glog/bin /home/pdell/dev/viz3d/external/../bin
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-build

glog/src/glog-stamp/glog-configure: glog/tmp/glog-cfgcmd.txt
glog/src/glog-stamp/glog-configure: glog/src/glog-stamp/glog-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'glog'"
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake -DCMAKE_INSTALL_PREFIX=/home/pdell/dev/viz3d/install/Release/glog "-GUnix Makefiles" /home/pdell/dev/viz3d/glog/src/glog
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-configure

glog/src/glog-stamp/glog-download: glog/src/glog-stamp/glog-gitinfo.txt
glog/src/glog-stamp/glog-download: glog/src/glog-stamp/glog-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'glog'"
	cd /home/pdell/dev/viz3d/glog/src && /usr/bin/cmake -P /home/pdell/dev/viz3d/glog/tmp/glog-gitclone.cmake
	cd /home/pdell/dev/viz3d/glog/src && /usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-download

glog/src/glog-stamp/glog-install: glog/src/glog-stamp/glog-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing install step for 'glog'"
	cd /home/pdell/dev/viz3d/glog/src/glog-build && $(MAKE) install
	cd /home/pdell/dev/viz3d/glog/src/glog-build && /usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-install

glog/src/glog-stamp/glog-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'glog'"
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/src/glog
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/src/glog-build
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/install/Release/glog
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/tmp
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/src/glog-stamp
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/src
	/usr/bin/cmake -E make_directory /home/pdell/dev/viz3d/glog/src/glog-stamp
	/usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-mkdir

glog/src/glog-stamp/glog-patch: glog/src/glog-stamp/glog-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'glog'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/pdell/dev/viz3d/glog/src/glog-stamp/glog-patch

glog/src/glog-stamp/glog-update: glog/src/glog-stamp/glog-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pdell/dev/viz3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing update step for 'glog'"
	cd /home/pdell/dev/viz3d/glog/src/glog && /usr/bin/cmake -P /home/pdell/dev/viz3d/glog/tmp/glog-gitupdate.cmake

glog: CMakeFiles/glog
glog: CMakeFiles/glog-complete
glog: glog/src/glog-stamp/glog-build
glog: glog/src/glog-stamp/glog-configure
glog: glog/src/glog-stamp/glog-download
glog: glog/src/glog-stamp/glog-install
glog: glog/src/glog-stamp/glog-mkdir
glog: glog/src/glog-stamp/glog-patch
glog: glog/src/glog-stamp/glog-update
glog: CMakeFiles/glog.dir/build.make
.PHONY : glog

# Rule to build all files generated by this target.
CMakeFiles/glog.dir/build: glog
.PHONY : CMakeFiles/glog.dir/build

CMakeFiles/glog.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glog.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glog.dir/clean

CMakeFiles/glog.dir/depend:
	cd /home/pdell/dev/viz3d && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pdell/dev/viz3d/external /home/pdell/dev/viz3d/external /home/pdell/dev/viz3d /home/pdell/dev/viz3d /home/pdell/dev/viz3d/CMakeFiles/glog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glog.dir/depend

