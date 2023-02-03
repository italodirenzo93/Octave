#ifndef OCTAVE_AUDIO_BUFFER_HPP
#define OCTAVE_AUDIO_BUFFER_HPP

#include "CommonInclude.hpp"

namespace octave::audio {

class Buffer {
	friend class Source;

public:
	Buffer();
	~Buffer() noexcept;

private:
	uint32_t id_ = 0;
};

}  // namespace octave

#endif  // OCTAVE_AUDIO_BUFFER_HPP
