cmake_minimum_required(VERSION 3.3)
project(Octave)

set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Indication to the code that this is a debug build
if (${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif ()

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(src ${OPENGL_INCLUDE_DIRS})

# Address Sanitizer
if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif ()

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)

add_subdirectory(dependencies)

# Main executable
add_executable(Octave "")
add_subdirectory(src)
target_link_libraries(Octave PRIVATE ${OPENGL_LIBRARIES} assimp glad glfw glm ImGui OpenAL simpleini stb_image)

# Visual Studio Startup Project
if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Octave)
endif ()

# Symlink Resources
add_custom_command(TARGET Octave PRE_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Octave>/resources)
