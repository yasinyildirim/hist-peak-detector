# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project ("hist-peaks")

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

# Include sub-projects.
add_subdirectory ("hist-peaks")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hist-peaks/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/hist-peaks/ )