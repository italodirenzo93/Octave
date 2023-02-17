add_subdirectory(audio)
add_subdirectory(graphics)
add_subdirectory(helpers)
add_subdirectory(input)
add_subdirectory(resources)
add_subdirectory(samples)
add_subdirectory(scene)

set(sources Camera.cpp Camera.hpp CommonInclude.hpp Config.cpp Config.hpp Exception.cpp Exception.hpp Main.cpp Octave.cpp Octave.hpp)
list(TRANSFORM sources PREPEND "src/")

target_sources(Octave PRIVATE ${sources})
