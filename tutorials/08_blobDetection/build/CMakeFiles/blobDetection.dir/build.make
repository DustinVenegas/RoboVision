# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/RoboVision/tutorials/08_blobDetection

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/RoboVision/tutorials/08_blobDetection/build

# Include any dependencies generated for this target.
include CMakeFiles/blobDetection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/blobDetection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/blobDetection.dir/flags.make

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o: CMakeFiles/blobDetection.dir/flags.make
CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o: ../src/blobDetection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/RoboVision/tutorials/08_blobDetection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o -c /home/pi/RoboVision/tutorials/08_blobDetection/src/blobDetection.cpp

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/blobDetection.dir/src/blobDetection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/RoboVision/tutorials/08_blobDetection/src/blobDetection.cpp > CMakeFiles/blobDetection.dir/src/blobDetection.cpp.i

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/blobDetection.dir/src/blobDetection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/RoboVision/tutorials/08_blobDetection/src/blobDetection.cpp -o CMakeFiles/blobDetection.dir/src/blobDetection.cpp.s

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.requires:

.PHONY : CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.requires

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.provides: CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.requires
	$(MAKE) -f CMakeFiles/blobDetection.dir/build.make CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.provides.build
.PHONY : CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.provides

CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.provides.build: CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o


# Object files for target blobDetection
blobDetection_OBJECTS = \
"CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o"

# External object files for target blobDetection
blobDetection_EXTERNAL_OBJECTS =

blobDetection: CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o
blobDetection: CMakeFiles/blobDetection.dir/build.make
blobDetection: /usr/local/lib/libopencv_shape.so.3.2.0
blobDetection: /usr/local/lib/libopencv_stitching.so.3.2.0
blobDetection: /usr/local/lib/libopencv_superres.so.3.2.0
blobDetection: /usr/local/lib/libopencv_videostab.so.3.2.0
blobDetection: /usr/local/lib/libopencv_objdetect.so.3.2.0
blobDetection: /usr/local/lib/libopencv_calib3d.so.3.2.0
blobDetection: /usr/local/lib/libopencv_features2d.so.3.2.0
blobDetection: /usr/local/lib/libopencv_flann.so.3.2.0
blobDetection: /usr/local/lib/libopencv_highgui.so.3.2.0
blobDetection: /usr/local/lib/libopencv_ml.so.3.2.0
blobDetection: /usr/local/lib/libopencv_photo.so.3.2.0
blobDetection: /usr/local/lib/libopencv_video.so.3.2.0
blobDetection: /usr/local/lib/libopencv_videoio.so.3.2.0
blobDetection: /usr/local/lib/libopencv_imgcodecs.so.3.2.0
blobDetection: /usr/local/lib/libopencv_imgproc.so.3.2.0
blobDetection: /usr/local/lib/libopencv_core.so.3.2.0
blobDetection: CMakeFiles/blobDetection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/RoboVision/tutorials/08_blobDetection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable blobDetection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blobDetection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/blobDetection.dir/build: blobDetection

.PHONY : CMakeFiles/blobDetection.dir/build

CMakeFiles/blobDetection.dir/requires: CMakeFiles/blobDetection.dir/src/blobDetection.cpp.o.requires

.PHONY : CMakeFiles/blobDetection.dir/requires

CMakeFiles/blobDetection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blobDetection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blobDetection.dir/clean

CMakeFiles/blobDetection.dir/depend:
	cd /home/pi/RoboVision/tutorials/08_blobDetection/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/RoboVision/tutorials/08_blobDetection /home/pi/RoboVision/tutorials/08_blobDetection /home/pi/RoboVision/tutorials/08_blobDetection/build /home/pi/RoboVision/tutorials/08_blobDetection/build /home/pi/RoboVision/tutorials/08_blobDetection/build/CMakeFiles/blobDetection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blobDetection.dir/depend

