# CMakeList.txt : CMake project for hist-peaks, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (hist-peaks "main.cpp" 
"DominantPeakFinder.cpp" "DominantPeakFinder.h" "libbmp/libbmp.cpp" "libbmp/libbmp.h")

# TODO: Add tests and install targets if needed.
