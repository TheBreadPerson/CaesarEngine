cmake_minimum_required(VERSION 3.29.5)

# Set policies (optional but recommended for future compatibility)
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Project setup
project(CaesarEngine VERSION 1.0 LANGUAGES CXX)

# Set C++23 as the language standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the compiler flags for MSVC
if(MSVC)
    add_compile_options(/permissive-)
endif()

# FetchContent module to automatically download dependencies
include(FetchContent)

# Cache external libraries in the 'thirdparty' folder
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# fastgltf
FetchContent_Declare(
    fastgltf
    GIT_REPOSITORY https://github.com/spnda/fastgltf.git
    GIT_TAG main
)
FetchContent_MakeAvailable(fastgltf)

# glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8  # Use a specific stable version
)
FetchContent_MakeAvailable(glm)

# glfw
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8  # Use a specific stable version
)
FetchContent_MakeAvailable(glfw)

# glad
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/backends)
target_sources(imgui PRIVATE 
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "source/*.cpp")

# Add executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} "source/debug.cpp")

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fastgltf glm glfw glad imgui)

# Include directories for stb and glad
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR} ${glad_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${glfw_SOURCE_DIR}/include)

# Add executable properties (optional)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
)

# Copy assets after build
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)
