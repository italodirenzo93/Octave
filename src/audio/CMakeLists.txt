set(sources alCommon.hpp Buffer.cpp Buffer.hpp Listener.cpp Listener.hpp Source.cpp Source.hpp stb_vorbis.c)
list(TRANSFORM sources PREPEND "src/audio/")

target_sources(Octave PRIVATE ${sources})
