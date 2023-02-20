set(sources InputSystem.hpp Gamepad.hpp)

# GLFW
list(APPEND sources glfw/InputSystemGLFW.cpp glfw/InputSystemGLFW.hpp glfw/GamepadGLFW.cpp glfw/GamepadGLFW.hpp)

list(TRANSFORM sources PREPEND "src/input/")
target_sources(Octave PRIVATE ${sources})
