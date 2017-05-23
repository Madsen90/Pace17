cmake_minimum_required (VERSION 2.8.11)
project (mfi)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -D NOEXPORT")

file(GLOB CPPACE_LIB_SOURCES CPPace/CPPace.Lib/*.cpp)
file(GLOB CPPACE_LIB_HEADERS CPPace/CPPace.Lib/*.h)
file(GLOB CPPACE_SOURCES CPPace/CPPace/*.cpp)
file(GLOB CPPACE_HEADERS CPPace/CPPace/*.h)

add_executable(mfi ${CPPACE_LIB_SOURCES} ${CPPACE_LIB_HEADERS} ${CPPACE_SOURCES} ${CPPACE_HEADERS})
