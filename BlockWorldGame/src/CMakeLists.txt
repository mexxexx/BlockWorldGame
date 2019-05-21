# Source Files
set (SOURCES main.cpp)
# Header Files
set (HEADER_PATH ${PROJECT_SOURCE_DIR}/BlockWorldGame/include)
set (HEADER )

# Create Executable
set(EXECUTABLE_NAME bwg)
add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADER})

# Include Project Header Files
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${HEADER_PATH})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/Engine/include)
# Link with Engine
target_link_libraries(${EXECUTABLE_NAME} engine)

# Include glad Header Files
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/ExternalLibraries/glad/include)
# Link with Glad
#target_link_libraries(${EXECUTABLE_NAME} glad)

# Link with GLFW
#find_package(glfw3 3.3 REQUIRED)
#target_link_libraries(${EXECUTABLE_NAME} glfw)