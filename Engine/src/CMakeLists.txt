# Source Files
set (SOURCES game.cpp)
# Header Files
set (HEADER_PATH ${PROJECT_SOURCE_DIR}/Engine/include)
set (HEADER ${HEADER_PATH}/engine.h ${HEADER_PATH}/game.h)

# Create Library
set (LIBRARY_NAME engine)
add_library(${LIBRARY_NAME} ${SOURCES} ${HEADER})

# Include Project Header Files
target_include_directories(${LIBRARY_NAME} PUBLIC ${HEADER_PATH})

# Include glad Header Files
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/ExternalLibraries/glad/include)

# Link with Glad
target_link_libraries(${LIBRARY_NAME} PUBLIC glad)

#GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${LIBRARY_NAME} PUBLIC glfw)