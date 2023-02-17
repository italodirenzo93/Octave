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

# Re-evaluate this option later...
set(BUILD_SHARED_LIBS OFF)
set(LIBTYPE STATIC)

# GLFW
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_USE_HYBRID_HPG ON)

# Assimp
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)

# OpenAL Soft
set(ALSOFT_UTILS OFF)
set(ALSOFT_EAX OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL_EXAMPLES OFF)
set(ALSOFT_INSTALL_UTILS OFF)
set(ALSOFT_UPDATE_BUILD_VERSION OFF)

# Subprojects
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/openal-soft)
add_subdirectory(dependencies/simpleini)
add_subdirectory(dependencies/stb_image)

add_executable(Octave "")
add_subdirectory(src)
target_link_libraries(Octave PRIVATE ${OPENGL_LIBRARIES} assimp glad glfw glm OpenAL simpleini stb_image)

# Visual Studio Startup Project
if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Octave)
endif ()

# Symlink Resources
add_custom_command(TARGET Octave PRE_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Octave>/resources)
