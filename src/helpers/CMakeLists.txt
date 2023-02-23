set(sources GeometricPrimitive.cpp GeometricPrimitive.hpp StepTimer.cpp StepTimer.hpp)
list(TRANSFORM sources PREPEND "src/helpers/")

target_sources(Octave PRIVATE ${sources})
