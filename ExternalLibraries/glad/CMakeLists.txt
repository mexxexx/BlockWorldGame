# Source Files
set (SOURCES src/glad.c)
# Header Files
set (HEADER include/glad/glad.h include/KHR/khrplatform.h)

# Include Project Header Files
include_directories(include)

#Create Library
add_library(glad ${SOURCES} ${HEADER})