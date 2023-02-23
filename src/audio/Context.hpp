#ifndef OCTAVE_AUDIO_CONTEXT_HPP
#define OCTAVE_AUDIO_CONTEXT_HPP

#include "CommonInclude.hpp"

namespace octave::audio {

class Context {
public:
	Context();
	~Context() noexcept;

private:
	ALCdevice* device_ = nullptr;
	ALCcontext* context_ = nullptr;
};

}  // namespace octave

#endif  // OCTAVE_AUDIO_CONTEXT_HPP
