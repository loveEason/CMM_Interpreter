cmake_minimum_required(VERSION 3.3)
project(CMM_Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Global.h LexAnalysis.h LexAnalysis.cpp Util.h Util.cpp Parse.h Parse.cpp)
add_executable(CMM_Interpreter ${SOURCE_FILES})
#${SOURCE_FILES}