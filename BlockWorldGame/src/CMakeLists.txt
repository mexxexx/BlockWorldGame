# Source Files
set (SOURCES main.cpp)
# Header Files
set (HEADER_PATH ${PROJECT_SOURCE_DIR}/BlockWorldGame/include)
set (HEADER )

# Include Project Header Files
include_directories(${HEADER_PATH})

# Include glad Header Files
include_directories(${PROJECT_SOURCE_DIR}/ExternalLibraries/glad/include)

# Create Executable
add_executable(BlockWorldGame ${SOURCES} ${HEADER})

# Link with Glad
target_link_libraries(BlockWorldGame glad)

#GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(BlockWorldGame glfw)