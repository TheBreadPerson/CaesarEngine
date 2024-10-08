cmake_minimum_required(VERSION 3.29.5)

# Project setup
project(CaesarEngine VERSION 1.0 LANGUAGES CXX)

# Set C++23 as the language standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 23)

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
    GIT_TAG main  # Use the latest version
)
FetchContent_MakeAvailable(fastgltf)

# glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master  # Use the latest version
)
FetchContent_MakeAvailable(glm)

# glfw
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master  # Use the latest version
)
FetchContent_MakeAvailable(glfw)

# glad
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master  # Use the latest version
)
FetchContent_MakeAvailable(glad)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master  # Use the latest version
)
FetchContent_MakeAvailable(stb)

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fastgltf glm glfw glad)

# Include directories for stb and glad
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR} ${glad_SOURCE_DIR})

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${PROJECT_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)