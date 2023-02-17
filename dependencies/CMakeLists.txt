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
add_subdirectory(assimp)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(openal-soft)
add_subdirectory(simpleini)
add_subdirectory(stb_image)

# ImGui
set(ImGui_OpenGL_Glfw_Sources imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_glfw.cpp)
add_library(ImGui imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp ${ImGui_OpenGL_Glfw_Sources})
target_include_directories(ImGui PUBLIC imgui imgui/backends)
target_link_libraries(ImGui PRIVATE glfw)
