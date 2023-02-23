set(sources Gamepad.cpp Gamepad.hpp Keyboard.cpp Keyboard.hpp)
list(TRANSFORM sources PREPEND "src/input/")

target_sources(Octave PRIVATE ${sources})
