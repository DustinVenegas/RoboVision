file (GLOB SOURCES
    src/*.cpp
    include/*.cpp
)
cmake_minimum_required(VERSION 2.8)
project( roboVision )
include_directories( include )
find_package( OpenCV REQUIRED )
add_executable( roboVision ${SOURCES} )
add_definitions(-std=c++0x)
target_link_libraries( roboVision ${OpenCV_LIBS} )
