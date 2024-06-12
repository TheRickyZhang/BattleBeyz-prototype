cmake_minimum_required(VERSION 3.10)
project(BattleBeyz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Disable Doxygen documentation
set(BUILD_DOCS OFF CACHE BOOL "Build documentation" FORCE)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glew-2.2.0/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw-3.4/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm-1.0.1)
include_directories(${PROJECT_SOURCE_DIR}/lib/freetype-2.13.2/include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Find and include OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Add GLFW
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/lib/glfw-3.4)
add_subdirectory(${GLFW_DIR})

# Add GLEW
set(GLEW_DIR ${PROJECT_SOURCE_DIR}/lib/glew-2.2.0)
link_directories(${GLEW_DIR}/lib/Release/x64)  # Adjust the path if needed
add_definitions(-DGLEW_STATIC)

# Manually specify Freetype paths
set(FREETYPE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/freetype-2.13.2/include")
set(FREETYPE_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/freetype-2.13.2/dll/win64/freetype.lib")
include_directories(${FREETYPE_INCLUDE_DIRS})

# Add external libraries
set(LIBS glfw glew32s ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} glu32 opengl32)

# Add assets directory to be copied to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add executable
add_executable(BattleBeyz ${SOURCES}
        src/ShaderProgram.cpp
        src/ShaderProgram.h
        src/Buffers.cpp
        src/Buffers.h
        src/Initialize.cpp
        src/Initialize.h
        src/TextRenderer.cpp
        src/TextRenderer.h
        src/ShaderSource.cpp
        src/ShaderSource.h
)

# Link libraries
target_link_libraries(BattleBeyz ${LIBS})
