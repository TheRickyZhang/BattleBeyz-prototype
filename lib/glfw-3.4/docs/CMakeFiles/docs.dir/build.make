# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz"

# Utility rule file for docs.

# Include any custom commands dependencies for this target.
include lib/glfw-3.4/docs/CMakeFiles/docs.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/glfw-3.4/docs/CMakeFiles/docs.dir/progress.make

lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/Doxyfile
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/include/GLFW/glfw3.h
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/include/GLFW/glfw3native.h
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/main.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/news.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/quick.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/moving.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/compile.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/build.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/intro.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/context.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/monitor.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/window.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/input.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/vulkan.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/compat.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/internal.md
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/DoxygenLayout.xml
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/header.html
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/footer.html
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/extra.css
lib/glfw-3.4/docs/html/index.html: lib/glfw-3.4/docs/spaces.svg
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating HTML documentation"
	cd /d C:\Users\ricky\ONEDRI~1\DOCUME~1\BATTLE~1\lib\glfw-3.4\docs && "C:\Program Files\doxygen\bin\doxygen.exe"

docs: lib/glfw-3.4/docs/html/index.html
docs: lib/glfw-3.4/docs/CMakeFiles/docs.dir/build.make
.PHONY : docs

# Rule to build all files generated by this target.
lib/glfw-3.4/docs/CMakeFiles/docs.dir/build: docs
.PHONY : lib/glfw-3.4/docs/CMakeFiles/docs.dir/build

lib/glfw-3.4/docs/CMakeFiles/docs.dir/clean:
	cd /d C:\Users\ricky\ONEDRI~1\DOCUME~1\BATTLE~1\lib\glfw-3.4\docs && $(CMAKE_COMMAND) -P CMakeFiles\docs.dir\cmake_clean.cmake
.PHONY : lib/glfw-3.4/docs/CMakeFiles/docs.dir/clean

lib/glfw-3.4/docs/CMakeFiles/docs.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz" "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz\lib\glfw-3.4\docs" "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz" "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz\lib\glfw-3.4\docs" "C:\Users\ricky\OneDrive - University of Florida\Documents\BattleBeyz\lib\glfw-3.4\docs\CMakeFiles\docs.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : lib/glfw-3.4/docs/CMakeFiles/docs.dir/depend
