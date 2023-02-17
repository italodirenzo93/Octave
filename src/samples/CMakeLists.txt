set(sources ModelViewerSample.cpp ModelViewerSample.hpp Sample.cpp Sample.hpp)
list(TRANSFORM sources PREPEND "src/samples/")

target_sources(Octave PRIVATE ${sources})
