cmake_minimum_required(VERSION 3.10)
project(BattleBeyz)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glew-2.2.0/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw-3.4/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm-1.0.1)

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

# Add external libraries
set(LIBS glfw glew32s ${OPENGL_LIBRARIES} glu32 opengl32)

# Add assets directory to be copied to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add executable
add_executable(BattleBeyz ${SOURCES})

# Link libraries
target_link_libraries(BattleBeyz ${LIBS})
