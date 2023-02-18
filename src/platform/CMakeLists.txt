set(sources Platform.hpp Window.hpp)

# GLFW
list(APPEND sources glfw/PlatformGLFW.cpp glfw/PlatformGLFW.hpp glfw/GLFWError.cpp glfw/GLFWError.hpp glfw/WindowGLFW.cpp glfw/WindowGLFW.hpp)

list(TRANSFORM sources PREPEND "src/platform/")

target_sources(Octave PRIVATE ${sources})
