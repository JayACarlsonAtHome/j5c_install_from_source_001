# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = "/Users/jaycarlson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/173.3727.114/CLion.app/Contents/bin/cmake/bin/cmake"

# The command to remove a file.
RM = "/Users/jaycarlson/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/173.3727.114/CLion.app/Contents/bin/cmake/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jaycarlson/git/j5c_install_from_source_001

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Install.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Install.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Install.dir/flags.make

CMakeFiles/Install.dir/main.cpp.o: CMakeFiles/Install.dir/flags.make
CMakeFiles/Install.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Install.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Install.dir/main.cpp.o -c /Users/jaycarlson/git/j5c_install_from_source_001/main.cpp

CMakeFiles/Install.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Install.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jaycarlson/git/j5c_install_from_source_001/main.cpp > CMakeFiles/Install.dir/main.cpp.i

CMakeFiles/Install.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Install.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jaycarlson/git/j5c_install_from_source_001/main.cpp -o CMakeFiles/Install.dir/main.cpp.s

CMakeFiles/Install.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Install.dir/main.cpp.o.requires

CMakeFiles/Install.dir/main.cpp.o.provides: CMakeFiles/Install.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Install.dir/build.make CMakeFiles/Install.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Install.dir/main.cpp.o.provides

CMakeFiles/Install.dir/main.cpp.o.provides.build: CMakeFiles/Install.dir/main.cpp.o


# Object files for target Install
Install_OBJECTS = \
"CMakeFiles/Install.dir/main.cpp.o"

# External object files for target Install
Install_EXTERNAL_OBJECTS =

Install: CMakeFiles/Install.dir/main.cpp.o
Install: CMakeFiles/Install.dir/build.make
Install: CMakeFiles/Install.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Install"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Install.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Install.dir/build: Install

.PHONY : CMakeFiles/Install.dir/build

CMakeFiles/Install.dir/requires: CMakeFiles/Install.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Install.dir/requires

CMakeFiles/Install.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Install.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Install.dir/clean

CMakeFiles/Install.dir/depend:
	cd /Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jaycarlson/git/j5c_install_from_source_001 /Users/jaycarlson/git/j5c_install_from_source_001 /Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug /Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug /Users/jaycarlson/git/j5c_install_from_source_001/cmake-build-debug/CMakeFiles/Install.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Install.dir/depend

