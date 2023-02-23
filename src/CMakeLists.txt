#add_subdirectory(audio)
add_subdirectory(graphics)
add_subdirectory(helpers)
add_subdirectory(input)
add_subdirectory(platform)
add_subdirectory(resources)
add_subdirectory(samples)
#add_subdirectory(scene)

set(sources pch.cpp Application.cpp Application.hpp Camera.cpp Camera.hpp Config.cpp Config.hpp Exception.cpp Exception.hpp Main.cpp Octave.hpp)
list(TRANSFORM sources PREPEND "src/")

target_sources(Octave PRIVATE ${sources})
target_precompile_headers(Octave PRIVATE pch.hpp)
