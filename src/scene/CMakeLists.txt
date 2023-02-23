set(sources MeshComponent.cpp MeshComponent.hpp Object.cpp Object.hpp)
list(TRANSFORM sources PREPEND "src/scene/")

target_sources(Octave PRIVATE ${sources})
