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
include agent/CMakeFiles/flecs-agent-server.dir/depend.make

# Include the progress variables for this target.
include agent/CMakeFiles/flecs-agent-server.dir/progress.make

# Include the compile flags for this target's objects.
include agent/CMakeFiles/flecs-agent-server.dir/flags.make

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o: agent/CMakeFiles/flecs-agent-server.dir/flags.make
agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o: agent/agent.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pjawahar/flecs-rpc/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flecs-agent-server.dir/agent.cpp.o -c /home/pjawahar/flecs-rpc/build/agent/agent.cpp

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flecs-agent-server.dir/agent.cpp.i"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pjawahar/flecs-rpc/build/agent/agent.cpp > CMakeFiles/flecs-agent-server.dir/agent.cpp.i

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flecs-agent-server.dir/agent.cpp.s"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pjawahar/flecs-rpc/build/agent/agent.cpp -o CMakeFiles/flecs-agent-server.dir/agent.cpp.s

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.requires:
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.requires

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.provides: agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.requires
	$(MAKE) -f agent/CMakeFiles/flecs-agent-server.dir/build.make agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.provides.build
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.provides

agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.provides.build: agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o: agent/CMakeFiles/flecs-agent-server.dir/flags.make
agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o: ../agent/server.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pjawahar/flecs-rpc/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flecs-agent-server.dir/server.cpp.o -c /home/pjawahar/flecs-rpc/agent/server.cpp

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flecs-agent-server.dir/server.cpp.i"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pjawahar/flecs-rpc/agent/server.cpp > CMakeFiles/flecs-agent-server.dir/server.cpp.i

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flecs-agent-server.dir/server.cpp.s"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pjawahar/flecs-rpc/agent/server.cpp -o CMakeFiles/flecs-agent-server.dir/server.cpp.s

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.requires:
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.requires

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.provides: agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.requires
	$(MAKE) -f agent/CMakeFiles/flecs-agent-server.dir/build.make agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.provides.build
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.provides

agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.provides.build: agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o: agent/CMakeFiles/flecs-agent-server.dir/flags.make
agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o: ../agent/agentI.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pjawahar/flecs-rpc/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/flecs-agent-server.dir/agentI.cpp.o -c /home/pjawahar/flecs-rpc/agent/agentI.cpp

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flecs-agent-server.dir/agentI.cpp.i"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pjawahar/flecs-rpc/agent/agentI.cpp > CMakeFiles/flecs-agent-server.dir/agentI.cpp.i

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flecs-agent-server.dir/agentI.cpp.s"
	cd /home/pjawahar/flecs-rpc/build/agent && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pjawahar/flecs-rpc/agent/agentI.cpp -o CMakeFiles/flecs-agent-server.dir/agentI.cpp.s

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.requires:
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.requires

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.provides: agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.requires
	$(MAKE) -f agent/CMakeFiles/flecs-agent-server.dir/build.make agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.provides.build
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.provides

agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.provides.build: agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o

agent/agent.h: ../agent/agent.ice
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pjawahar/flecs-rpc/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating agent.h, agent.cpp"
	cd /home/pjawahar/flecs-rpc/build/agent && slice2cpp /home/pjawahar/flecs-rpc/agent/agent.ice

agent/agent.cpp: agent/agent.h

# Object files for target flecs-agent-server
flecs__agent__server_OBJECTS = \
"CMakeFiles/flecs-agent-server.dir/agent.cpp.o" \
"CMakeFiles/flecs-agent-server.dir/server.cpp.o" \
"CMakeFiles/flecs-agent-server.dir/agentI.cpp.o"

# External object files for target flecs-agent-server
flecs__agent__server_EXTERNAL_OBJECTS =

agent/flecs-agent-server: agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o
agent/flecs-agent-server: agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o
agent/flecs-agent-server: agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o
agent/flecs-agent-server: /usr/lib/libboost_filesystem-mt.so
agent/flecs-agent-server: /usr/lib/libboost_system-mt.so
agent/flecs-agent-server: /usr/lib/libboost_program_options-mt.so
agent/flecs-agent-server: /usr/lib/libboost_thread-mt.so
agent/flecs-agent-server: agent/CMakeFiles/flecs-agent-server.dir/build.make
agent/flecs-agent-server: agent/CMakeFiles/flecs-agent-server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable flecs-agent-server"
	cd /home/pjawahar/flecs-rpc/build/agent && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flecs-agent-server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
agent/CMakeFiles/flecs-agent-server.dir/build: agent/flecs-agent-server
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/build

agent/CMakeFiles/flecs-agent-server.dir/requires: agent/CMakeFiles/flecs-agent-server.dir/agent.cpp.o.requires
agent/CMakeFiles/flecs-agent-server.dir/requires: agent/CMakeFiles/flecs-agent-server.dir/server.cpp.o.requires
agent/CMakeFiles/flecs-agent-server.dir/requires: agent/CMakeFiles/flecs-agent-server.dir/agentI.cpp.o.requires
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/requires

agent/CMakeFiles/flecs-agent-server.dir/clean:
	cd /home/pjawahar/flecs-rpc/build/agent && $(CMAKE_COMMAND) -P CMakeFiles/flecs-agent-server.dir/cmake_clean.cmake
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/clean

agent/CMakeFiles/flecs-agent-server.dir/depend: agent/agent.h
agent/CMakeFiles/flecs-agent-server.dir/depend: agent/agent.cpp
	cd /home/pjawahar/flecs-rpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pjawahar/flecs-rpc /home/pjawahar/flecs-rpc/agent /home/pjawahar/flecs-rpc/build /home/pjawahar/flecs-rpc/build/agent /home/pjawahar/flecs-rpc/build/agent/CMakeFiles/flecs-agent-server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : agent/CMakeFiles/flecs-agent-server.dir/depend

