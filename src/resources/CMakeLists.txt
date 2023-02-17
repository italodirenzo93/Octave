set(sources ResourceManager.hpp ShaderManager.cpp ShaderManager.hpp)
list(TRANSFORM sources PREPEND "src/resources/")

target_sources(Octave PRIVATE ${sources})
