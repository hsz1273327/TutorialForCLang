# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception

# Include any dependencies generated for this target.
include CMakeFiles/exception.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/exception.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exception.dir/flags.make

CMakeFiles/exception.dir/src/assert.c.o: CMakeFiles/exception.dir/flags.make
CMakeFiles/exception.dir/src/assert.c.o: src/assert.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/exception.dir/src/assert.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/exception.dir/src/assert.c.o   -c /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/assert.c

CMakeFiles/exception.dir/src/assert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/exception.dir/src/assert.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/assert.c > CMakeFiles/exception.dir/src/assert.c.i

CMakeFiles/exception.dir/src/assert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/exception.dir/src/assert.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/assert.c -o CMakeFiles/exception.dir/src/assert.c.s

CMakeFiles/exception.dir/src/assert.c.o.requires:

.PHONY : CMakeFiles/exception.dir/src/assert.c.o.requires

CMakeFiles/exception.dir/src/assert.c.o.provides: CMakeFiles/exception.dir/src/assert.c.o.requires
	$(MAKE) -f CMakeFiles/exception.dir/build.make CMakeFiles/exception.dir/src/assert.c.o.provides.build
.PHONY : CMakeFiles/exception.dir/src/assert.c.o.provides

CMakeFiles/exception.dir/src/assert.c.o.provides.build: CMakeFiles/exception.dir/src/assert.c.o


CMakeFiles/exception.dir/src/except.c.o: CMakeFiles/exception.dir/flags.make
CMakeFiles/exception.dir/src/except.c.o: src/except.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/exception.dir/src/except.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/exception.dir/src/except.c.o   -c /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/except.c

CMakeFiles/exception.dir/src/except.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/exception.dir/src/except.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/except.c > CMakeFiles/exception.dir/src/except.c.i

CMakeFiles/exception.dir/src/except.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/exception.dir/src/except.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/src/except.c -o CMakeFiles/exception.dir/src/except.c.s

CMakeFiles/exception.dir/src/except.c.o.requires:

.PHONY : CMakeFiles/exception.dir/src/except.c.o.requires

CMakeFiles/exception.dir/src/except.c.o.provides: CMakeFiles/exception.dir/src/except.c.o.requires
	$(MAKE) -f CMakeFiles/exception.dir/build.make CMakeFiles/exception.dir/src/except.c.o.provides.build
.PHONY : CMakeFiles/exception.dir/src/except.c.o.provides

CMakeFiles/exception.dir/src/except.c.o.provides.build: CMakeFiles/exception.dir/src/except.c.o


# Object files for target exception
exception_OBJECTS = \
"CMakeFiles/exception.dir/src/assert.c.o" \
"CMakeFiles/exception.dir/src/except.c.o"

# External object files for target exception
exception_EXTERNAL_OBJECTS =

lib/libexception.dylib: CMakeFiles/exception.dir/src/assert.c.o
lib/libexception.dylib: CMakeFiles/exception.dir/src/except.c.o
lib/libexception.dylib: CMakeFiles/exception.dir/build.make
lib/libexception.dylib: CMakeFiles/exception.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library lib/libexception.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exception.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exception.dir/build: lib/libexception.dylib

.PHONY : CMakeFiles/exception.dir/build

CMakeFiles/exception.dir/requires: CMakeFiles/exception.dir/src/assert.c.o.requires
CMakeFiles/exception.dir/requires: CMakeFiles/exception.dir/src/except.c.o.requires

.PHONY : CMakeFiles/exception.dir/requires

CMakeFiles/exception.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exception.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exception.dir/clean

CMakeFiles/exception.dir/depend:
	cd /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception /Users/huangsizhe/WORKSPACE/Blog/Docs/C_and_Cpp/TutorialForCpp/ipynbs/codes/C2_exception/exception/CMakeFiles/exception.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/exception.dir/depend

