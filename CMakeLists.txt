cmake_minimum_required(VERSION 3.12)
project( Puzzle_Game_with_Cpp_and_OpenCV )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( puzzle puzzle.cpp )
target_link_libraries( puzzle ${OpenCV_LIBS} )
