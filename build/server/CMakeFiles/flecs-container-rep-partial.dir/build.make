# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pjawahar/flecs-rpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pjawahar/flecs-rpc/build

# Include any dependencies generated for this target.
include server/CMakeFiles/flecs-container-rep-partial.dir/depend.make

# Include the progress variables for this target.
include server/CMakeFiles/flecs-container-rep-partial.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/flecs-container-rep-partial.dir/flags.make

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o: server/CMakeFiles/flecs-container-rep-partial.dir/flags.make
server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o: ../server/container-rep-partial.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pjawahar/flecs-rpc/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o"
	cd /home/pjawahar/flecs-rpc/build/server && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o -c /home/pjawahar/flecs-rpc/server/container-rep-partial.cpp

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.i"
	cd /home/pjawahar/flecs-rpc/build/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pjawahar/flecs-rpc/server/container-rep-partial.cpp > CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.i

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.s"
	cd /home/pjawahar/flecs-rpc/build/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pjawahar/flecs-rpc/server/container-rep-partial.cpp -o CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.s

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.requires:
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.requires

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.provides: server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.requires
	$(MAKE) -f server/CMakeFiles/flecs-container-rep-partial.dir/build.make server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.provides.build
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.provides

server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.provides.build: server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o

# Object files for target flecs-container-rep-partial
flecs__container__rep__partial_OBJECTS = \
"CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o"

# External object files for target flecs-container-rep-partial
flecs__container__rep__partial_EXTERNAL_OBJECTS =

server/libflecs-container-rep-partial.so: server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o
server/libflecs-container-rep-partial.so: server/CMakeFiles/flecs-container-rep-partial.dir/build.make
server/libflecs-container-rep-partial.so: server/CMakeFiles/flecs-container-rep-partial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libflecs-container-rep-partial.so"
	cd /home/pjawahar/flecs-rpc/build/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flecs-container-rep-partial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/flecs-container-rep-partial.dir/build: server/libflecs-container-rep-partial.so
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/build

server/CMakeFiles/flecs-container-rep-partial.dir/requires: server/CMakeFiles/flecs-container-rep-partial.dir/container-rep-partial.cpp.o.requires
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/requires

server/CMakeFiles/flecs-container-rep-partial.dir/clean:
	cd /home/pjawahar/flecs-rpc/build/server && $(CMAKE_COMMAND) -P CMakeFiles/flecs-container-rep-partial.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/clean

server/CMakeFiles/flecs-container-rep-partial.dir/depend:
	cd /home/pjawahar/flecs-rpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pjawahar/flecs-rpc /home/pjawahar/flecs-rpc/server /home/pjawahar/flecs-rpc/build /home/pjawahar/flecs-rpc/build/server /home/pjawahar/flecs-rpc/build/server/CMakeFiles/flecs-container-rep-partial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/flecs-container-rep-partial.dir/depend

