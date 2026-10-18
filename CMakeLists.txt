cmake_minimum_required(VERSION 3.20)
project(PG2026_2)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    Project1
    Project1/core
    packages/glm.1.0.3/build/native/include
    packages/glfw.3.4.0/build/native/include
    packages/glew.v140.1.12.0/build/native/include
)

# Source files
set(SOURCES
    Project1/core/mesh.cpp
    Project1/core/shader.cpp
    Project1/core/window.cpp
    Project1/texture.cpp
    Project1/src/Lista 2/Ex6/main.cpp
)

add_executable(PG2026_2 ${SOURCES})

# Link libraries (Simplified - you might need to adjust paths for your specific setup)
link_directories(
    ${CMAKE_SOURCE_DIR}/packages/glfw.3.4.0/build/native/lib/static/v143/x64
    ${CMAKE_SOURCE_DIR}/packages/glew.v140.1.12.0/build/native/lib/x64/Release
)

target_link_libraries(PG2026_2 opengl32 glfw3 glew32s)
