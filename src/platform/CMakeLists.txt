set(sources Platform.hpp Window.cpp Window.hpp)

# GLFW
list(APPEND sources glfw/PlatformGlfw.cpp glfw/PlatformGlfw.hpp glfw/GlfwError.cpp glfw/GlfwError.hpp glfw/GlfwWindow.cpp glfw/GlfwWindow.hpp)

list(TRANSFORM sources PREPEND "src/platform/")

target_sources(Octave PRIVATE ${sources})
