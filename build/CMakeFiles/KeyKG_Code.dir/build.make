# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/triccsr/Documents/2023-0/GST/GSTcode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/triccsr/Documents/2023-0/GST/GSTcode/build

# Include any dependencies generated for this target.
include CMakeFiles/KeyKG_Code.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/KeyKG_Code.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/KeyKG_Code.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/KeyKG_Code.dir/flags.make

CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o: CMakeFiles/KeyKG_Code.dir/flags.make
CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o: /home/triccsr/Documents/2023-0/GST/GSTcode/src/bc.cpp
CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o: CMakeFiles/KeyKG_Code.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/triccsr/Documents/2023-0/GST/GSTcode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o -MF CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o.d -o CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o -c /home/triccsr/Documents/2023-0/GST/GSTcode/src/bc.cpp

CMakeFiles/KeyKG_Code.dir/src/bc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KeyKG_Code.dir/src/bc.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/triccsr/Documents/2023-0/GST/GSTcode/src/bc.cpp > CMakeFiles/KeyKG_Code.dir/src/bc.cpp.i

CMakeFiles/KeyKG_Code.dir/src/bc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KeyKG_Code.dir/src/bc.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/triccsr/Documents/2023-0/GST/GSTcode/src/bc.cpp -o CMakeFiles/KeyKG_Code.dir/src/bc.cpp.s

CMakeFiles/KeyKG_Code.dir/src/main.cpp.o: CMakeFiles/KeyKG_Code.dir/flags.make
CMakeFiles/KeyKG_Code.dir/src/main.cpp.o: /home/triccsr/Documents/2023-0/GST/GSTcode/src/main.cpp
CMakeFiles/KeyKG_Code.dir/src/main.cpp.o: CMakeFiles/KeyKG_Code.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/triccsr/Documents/2023-0/GST/GSTcode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/KeyKG_Code.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/KeyKG_Code.dir/src/main.cpp.o -MF CMakeFiles/KeyKG_Code.dir/src/main.cpp.o.d -o CMakeFiles/KeyKG_Code.dir/src/main.cpp.o -c /home/triccsr/Documents/2023-0/GST/GSTcode/src/main.cpp

CMakeFiles/KeyKG_Code.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KeyKG_Code.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/triccsr/Documents/2023-0/GST/GSTcode/src/main.cpp > CMakeFiles/KeyKG_Code.dir/src/main.cpp.i

CMakeFiles/KeyKG_Code.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KeyKG_Code.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/triccsr/Documents/2023-0/GST/GSTcode/src/main.cpp -o CMakeFiles/KeyKG_Code.dir/src/main.cpp.s

# Object files for target KeyKG_Code
KeyKG_Code_OBJECTS = \
"CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o" \
"CMakeFiles/KeyKG_Code.dir/src/main.cpp.o"

# External object files for target KeyKG_Code
KeyKG_Code_EXTERNAL_OBJECTS =

KeyKG_Code: CMakeFiles/KeyKG_Code.dir/src/bc.cpp.o
KeyKG_Code: CMakeFiles/KeyKG_Code.dir/src/main.cpp.o
KeyKG_Code: CMakeFiles/KeyKG_Code.dir/build.make
KeyKG_Code: CMakeFiles/KeyKG_Code.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/triccsr/Documents/2023-0/GST/GSTcode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable KeyKG_Code"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/KeyKG_Code.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/KeyKG_Code.dir/build: KeyKG_Code
.PHONY : CMakeFiles/KeyKG_Code.dir/build

CMakeFiles/KeyKG_Code.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/KeyKG_Code.dir/cmake_clean.cmake
.PHONY : CMakeFiles/KeyKG_Code.dir/clean

CMakeFiles/KeyKG_Code.dir/depend:
	cd /home/triccsr/Documents/2023-0/GST/GSTcode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/triccsr/Documents/2023-0/GST/GSTcode /home/triccsr/Documents/2023-0/GST/GSTcode /home/triccsr/Documents/2023-0/GST/GSTcode/build /home/triccsr/Documents/2023-0/GST/GSTcode/build /home/triccsr/Documents/2023-0/GST/GSTcode/build/CMakeFiles/KeyKG_Code.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/KeyKG_Code.dir/depend

