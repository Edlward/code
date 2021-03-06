# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/lxg/code/opencv/tool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lxg/code/opencv/tool/build

# Include any dependencies generated for this target.
include CMakeFiles/crop.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/crop.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/crop.dir/flags.make

CMakeFiles/crop.dir/crop.cpp.o: CMakeFiles/crop.dir/flags.make
CMakeFiles/crop.dir/crop.cpp.o: ../crop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxg/code/opencv/tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/crop.dir/crop.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crop.dir/crop.cpp.o -c /home/lxg/code/opencv/tool/crop.cpp

CMakeFiles/crop.dir/crop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crop.dir/crop.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxg/code/opencv/tool/crop.cpp > CMakeFiles/crop.dir/crop.cpp.i

CMakeFiles/crop.dir/crop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crop.dir/crop.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxg/code/opencv/tool/crop.cpp -o CMakeFiles/crop.dir/crop.cpp.s

CMakeFiles/crop.dir/crop.cpp.o.requires:

.PHONY : CMakeFiles/crop.dir/crop.cpp.o.requires

CMakeFiles/crop.dir/crop.cpp.o.provides: CMakeFiles/crop.dir/crop.cpp.o.requires
	$(MAKE) -f CMakeFiles/crop.dir/build.make CMakeFiles/crop.dir/crop.cpp.o.provides.build
.PHONY : CMakeFiles/crop.dir/crop.cpp.o.provides

CMakeFiles/crop.dir/crop.cpp.o.provides.build: CMakeFiles/crop.dir/crop.cpp.o


# Object files for target crop
crop_OBJECTS = \
"CMakeFiles/crop.dir/crop.cpp.o"

# External object files for target crop
crop_EXTERNAL_OBJECTS =

crop: CMakeFiles/crop.dir/crop.cpp.o
crop: CMakeFiles/crop.dir/build.make
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_ml.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_shape.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_stitching.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_superres.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_videostab.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_aruco.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_bgsegm.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_bioinspired.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_ccalib.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_dpm.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_face.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_fuzzy.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_img_hash.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_line_descriptor.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_phase_unwrapping.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_reg.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_rgbd.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_saliency.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_tracking.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_xobjdetect.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_xphoto.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_photo.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_calib3d.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_features2d.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_flann.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_highgui.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_videoio.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_dnn.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_video.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_plot.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_imgcodecs.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_objdetect.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_imgproc.so.3.3.0
crop: /home/lxg/app/opencv-3.3.0/build/lib/libopencv_core.so.3.3.0
crop: CMakeFiles/crop.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lxg/code/opencv/tool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable crop"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crop.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/crop.dir/build: crop

.PHONY : CMakeFiles/crop.dir/build

CMakeFiles/crop.dir/requires: CMakeFiles/crop.dir/crop.cpp.o.requires

.PHONY : CMakeFiles/crop.dir/requires

CMakeFiles/crop.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/crop.dir/cmake_clean.cmake
.PHONY : CMakeFiles/crop.dir/clean

CMakeFiles/crop.dir/depend:
	cd /home/lxg/code/opencv/tool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lxg/code/opencv/tool /home/lxg/code/opencv/tool /home/lxg/code/opencv/tool/build /home/lxg/code/opencv/tool/build /home/lxg/code/opencv/tool/build/CMakeFiles/crop.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/crop.dir/depend

