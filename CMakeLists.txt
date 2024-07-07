cmake_minimum_required(VERSION 3.10)
project(BattleBeyz)

# Must set build type to debug or release for physx
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -Wno-attributes -Wno-unknown-pragmas")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define _DEBUG or NDEBUG based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glew-2.2.0/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw-3.4/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm-1.0.1)
include_directories(${PROJECT_SOURCE_DIR}/lib/freetype-2.13.2/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/stb)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8)
include_directories(${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/backends)
include_directories(${PROJECT_SOURCE_DIR}/lib/assimp-5.2.5/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/physx-5.4.0/include)


# Specify ImGui source files
set(IMGUI_SOURCES
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/imgui.cpp
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/backends/imgui_impl_opengl3.cpp
        ${PROJECT_SOURCE_DIR}/lib/imgui-1.90.8/backends/imgui_impl_glfw.cpp
)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" ${IMGUI_SOURCES})

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

# Add Assimp
set(ASSIMP_LIB ${PROJECT_SOURCE_DIR}/lib/assimp-5.2.5/lib/x64/assimp-vc143-mt.lib)

# Add PhysX
link_directories(${PROJECT_SOURCE_DIR}/lib/physx-5.4.0/lib)

# Add external libraries
set(LIBS glfw glew32s ${OPENGL_LIBRARIES} freetype glu32 opengl32 ${ASSIMP_LIB}
        PhysX_64
        PhysXCommon_64
        PhysXFoundation_64
        PhysXPvdSDK_static_64
    )

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
        src/Texture.cpp
        src/Texture.h
        src/Camera.cpp
        src/Camera.h
        src/Callbacks.cpp
        src/Callbacks.h
        src/CallbackData.h
        src/UI.cpp
        src/UI.h
        src/QuadRenderer.cpp
        src/QuadRenderer.h
        src/Beyblade.cpp
        src/Beyblade.h
        src/BoundingBox.cpp
        src/BoundingBox.h
)

# Link libraries
target_link_libraries(BattleBeyz ${LIBS})
