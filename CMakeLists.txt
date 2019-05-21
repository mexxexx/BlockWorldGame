#CMake Version
cmake_minimum_required (VERSION 3.10)

#Project Configuration
project (BlockWorld) 
set (CMAKE_CXX_STANDARD 11)

#Subdirectories
add_subdirectory(BlockWorldGame)
add_subdirectory(ExternalLibraries/glad)