# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eyvy/test/c_challenge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eyvy/test/c_challenge/build

# Include any dependencies generated for this target.
include CMakeFiles/BackC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/BackC.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BackC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BackC.dir/flags.make

CMakeFiles/BackC.dir/src/main.c.o: CMakeFiles/BackC.dir/flags.make
CMakeFiles/BackC.dir/src/main.c.o: ../src/main.c
CMakeFiles/BackC.dir/src/main.c.o: CMakeFiles/BackC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eyvy/test/c_challenge/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/BackC.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/BackC.dir/src/main.c.o -MF CMakeFiles/BackC.dir/src/main.c.o.d -o CMakeFiles/BackC.dir/src/main.c.o -c /home/eyvy/test/c_challenge/src/main.c

CMakeFiles/BackC.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/BackC.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/eyvy/test/c_challenge/src/main.c > CMakeFiles/BackC.dir/src/main.c.i

CMakeFiles/BackC.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/BackC.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/eyvy/test/c_challenge/src/main.c -o CMakeFiles/BackC.dir/src/main.c.s

# Object files for target BackC
BackC_OBJECTS = \
"CMakeFiles/BackC.dir/src/main.c.o"

# External object files for target BackC
BackC_EXTERNAL_OBJECTS =

BackC: CMakeFiles/BackC.dir/src/main.c.o
BackC: CMakeFiles/BackC.dir/build.make
BackC: CMakeFiles/BackC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eyvy/test/c_challenge/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable BackC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BackC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BackC.dir/build: BackC
.PHONY : CMakeFiles/BackC.dir/build

CMakeFiles/BackC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BackC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BackC.dir/clean

CMakeFiles/BackC.dir/depend:
	cd /home/eyvy/test/c_challenge/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eyvy/test/c_challenge /home/eyvy/test/c_challenge /home/eyvy/test/c_challenge/build /home/eyvy/test/c_challenge/build /home/eyvy/test/c_challenge/build/CMakeFiles/BackC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BackC.dir/depend

