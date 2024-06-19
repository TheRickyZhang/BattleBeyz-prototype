cmake_minimum_required(VERSION 3.10)
project(BattleBeyz)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
link_directories(${GLEW_DIR}/lib/Release/x64)
add_definitions(-DGLEW_STATIC)

# Add FreeType
set(FREETYPE_DIR ${PROJECT_SOURCE_DIR}/lib/freetype-2.13.2)
link_directories(${FREETYPE_DIR}/dll/win64)

# Add external libraries
set(LIBS glfw glew32s ${OPENGL_LIBRARIES} freetype glu32 opengl32)

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
        src/ShaderPath.h
        src/Utils.cpp
        src/Utils.h
        src/GameObject.h
        src/Stadium.cpp
        src/Stadium.h
)

# Link libraries
target_link_libraries(BattleBeyz ${LIBS})
