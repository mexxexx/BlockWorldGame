# Source Files
set (SOURCES_GAME
        main.cpp 
        blockWorldGame.cpp
        terrain.cpp
        chunk.cpp
        chunkContainer.cpp
        valueNoise.cpp)
# Header Files
set (HEADER_PATH ${PROJECT_SOURCE_DIR}/BlockWorldGame/include)
set (HEADER_GAME
        ${HEADER_PATH}/blockWorldGame.h
        ${HEADER_PATH}/terrain.h
        ${HEADER_PATH}/chunk.h
        ${HEADER_PATH}/blockType.h
        ${HEADER_PATH}/chunkContainer.h
        ${HEADER_PATH}/valueNoise.h)
 
# Create Executable
set(EXECUTABLE_NAME bwg)
add_executable(${EXECUTABLE_NAME} ${SOURCES_GAME} ${HEADER_GAME})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Copy contents
add_custom_command(TARGET bwg POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/BlockWorldGame/content/ $<TARGET_FILE_DIR:bwg>/content)

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