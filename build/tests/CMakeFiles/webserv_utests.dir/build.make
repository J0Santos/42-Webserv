# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_SOURCE_DIR = /home/mlancac/.Cursus/lv5/my-webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mlancac/.Cursus/lv5/my-webserv/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/webserv_utests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/webserv_utests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/webserv_utests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/webserv_utests.dir/flags.make

tests/CMakeFiles/webserv_utests.dir/test.cpp.o: tests/CMakeFiles/webserv_utests.dir/flags.make
tests/CMakeFiles/webserv_utests.dir/test.cpp.o: /home/mlancac/.Cursus/lv5/my-webserv/tests/test.cpp
tests/CMakeFiles/webserv_utests.dir/test.cpp.o: tests/CMakeFiles/webserv_utests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mlancac/.Cursus/lv5/my-webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/webserv_utests.dir/test.cpp.o"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/webserv_utests.dir/test.cpp.o -MF CMakeFiles/webserv_utests.dir/test.cpp.o.d -o CMakeFiles/webserv_utests.dir/test.cpp.o -c /home/mlancac/.Cursus/lv5/my-webserv/tests/test.cpp

tests/CMakeFiles/webserv_utests.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv_utests.dir/test.cpp.i"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mlancac/.Cursus/lv5/my-webserv/tests/test.cpp > CMakeFiles/webserv_utests.dir/test.cpp.i

tests/CMakeFiles/webserv_utests.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv_utests.dir/test.cpp.s"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mlancac/.Cursus/lv5/my-webserv/tests/test.cpp -o CMakeFiles/webserv_utests.dir/test.cpp.s

tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o: tests/CMakeFiles/webserv_utests.dir/flags.make
tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o: /home/mlancac/.Cursus/lv5/my-webserv/tests/testLogger.cpp
tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o: tests/CMakeFiles/webserv_utests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mlancac/.Cursus/lv5/my-webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o -MF CMakeFiles/webserv_utests.dir/testLogger.cpp.o.d -o CMakeFiles/webserv_utests.dir/testLogger.cpp.o -c /home/mlancac/.Cursus/lv5/my-webserv/tests/testLogger.cpp

tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv_utests.dir/testLogger.cpp.i"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mlancac/.Cursus/lv5/my-webserv/tests/testLogger.cpp > CMakeFiles/webserv_utests.dir/testLogger.cpp.i

tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv_utests.dir/testLogger.cpp.s"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mlancac/.Cursus/lv5/my-webserv/tests/testLogger.cpp -o CMakeFiles/webserv_utests.dir/testLogger.cpp.s

tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o: tests/CMakeFiles/webserv_utests.dir/flags.make
tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o: /home/mlancac/.Cursus/lv5/my-webserv/tests/testSmt.cpp
tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o: tests/CMakeFiles/webserv_utests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mlancac/.Cursus/lv5/my-webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o -MF CMakeFiles/webserv_utests.dir/testSmt.cpp.o.d -o CMakeFiles/webserv_utests.dir/testSmt.cpp.o -c /home/mlancac/.Cursus/lv5/my-webserv/tests/testSmt.cpp

tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv_utests.dir/testSmt.cpp.i"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mlancac/.Cursus/lv5/my-webserv/tests/testSmt.cpp > CMakeFiles/webserv_utests.dir/testSmt.cpp.i

tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv_utests.dir/testSmt.cpp.s"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mlancac/.Cursus/lv5/my-webserv/tests/testSmt.cpp -o CMakeFiles/webserv_utests.dir/testSmt.cpp.s

tests/CMakeFiles/webserv_utests.dir/testString.cpp.o: tests/CMakeFiles/webserv_utests.dir/flags.make
tests/CMakeFiles/webserv_utests.dir/testString.cpp.o: /home/mlancac/.Cursus/lv5/my-webserv/tests/testString.cpp
tests/CMakeFiles/webserv_utests.dir/testString.cpp.o: tests/CMakeFiles/webserv_utests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mlancac/.Cursus/lv5/my-webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/webserv_utests.dir/testString.cpp.o"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/webserv_utests.dir/testString.cpp.o -MF CMakeFiles/webserv_utests.dir/testString.cpp.o.d -o CMakeFiles/webserv_utests.dir/testString.cpp.o -c /home/mlancac/.Cursus/lv5/my-webserv/tests/testString.cpp

tests/CMakeFiles/webserv_utests.dir/testString.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv_utests.dir/testString.cpp.i"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mlancac/.Cursus/lv5/my-webserv/tests/testString.cpp > CMakeFiles/webserv_utests.dir/testString.cpp.i

tests/CMakeFiles/webserv_utests.dir/testString.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv_utests.dir/testString.cpp.s"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mlancac/.Cursus/lv5/my-webserv/tests/testString.cpp -o CMakeFiles/webserv_utests.dir/testString.cpp.s

# Object files for target webserv_utests
webserv_utests_OBJECTS = \
"CMakeFiles/webserv_utests.dir/test.cpp.o" \
"CMakeFiles/webserv_utests.dir/testLogger.cpp.o" \
"CMakeFiles/webserv_utests.dir/testSmt.cpp.o" \
"CMakeFiles/webserv_utests.dir/testString.cpp.o"

# External object files for target webserv_utests
webserv_utests_EXTERNAL_OBJECTS =

tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/test.cpp.o
tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/testLogger.cpp.o
tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/testSmt.cpp.o
tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/testString.cpp.o
tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/build.make
tests/webserv_utests: src/libwebserv_lib.a
tests/webserv_utests: lib/libgtest_main.a
tests/webserv_utests: lib/libgtest.a
tests/webserv_utests: tests/CMakeFiles/webserv_utests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mlancac/.Cursus/lv5/my-webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable webserv_utests"
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserv_utests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/webserv_utests.dir/build: tests/webserv_utests
.PHONY : tests/CMakeFiles/webserv_utests.dir/build

tests/CMakeFiles/webserv_utests.dir/clean:
	cd /home/mlancac/.Cursus/lv5/my-webserv/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/webserv_utests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/webserv_utests.dir/clean

tests/CMakeFiles/webserv_utests.dir/depend:
	cd /home/mlancac/.Cursus/lv5/my-webserv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mlancac/.Cursus/lv5/my-webserv /home/mlancac/.Cursus/lv5/my-webserv/tests /home/mlancac/.Cursus/lv5/my-webserv/build /home/mlancac/.Cursus/lv5/my-webserv/build/tests /home/mlancac/.Cursus/lv5/my-webserv/build/tests/CMakeFiles/webserv_utests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/webserv_utests.dir/depend

