#!/usr/bin/env bash
# an automated way to make CMakeLists, because I'm lazy:
if [ $# -eq 0 ]
    then
        echo 'No Project name specified'
    else
        file=CMakeLists.txt
        printf 'cmake_minimum_required(VERSION 2.8)\n' >> $file
        printf "project( $1 )\n" >> $file
        printf 'find_package( OpenCV REQUIRED )\n' >> $file
        printf "add_executable( $1 $1.cpp )\n" >> $file
        printf "target_link_libraries( $1" >> $file
        printf ' ${OpenCV_LIBS} )\n'  >> $file
fi
