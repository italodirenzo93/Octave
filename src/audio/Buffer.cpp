#include "Buffer.hpp"

namespace octave::audio {

Buffer::Buffer() {
	alGenBuffers( 1, &id_ );
	// throw if error
}

Buffer::~Buffer() noexcept {
	alDeleteBuffers( 1, &id_ );
}

}  // namespace octave::audio
