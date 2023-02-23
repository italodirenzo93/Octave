#include "Context.hpp"

#include "Exception.hpp"

namespace octave::audio {

Context::Context() {
	device_ = alcOpenDevice( nullptr );  // default device
	if ( !device_ ) {
		throw Exception( "No usable audio device" );
	}

	context_ = alcCreateContext( device_, nullptr );
	alcMakeContextCurrent( context_ );

	// Clear error state
	alGetError();
}

Context::~Context() noexcept {
	alcDestroyContext( context_ );
	alcCloseDevice( device_ );
}

}  // namespace octave::audio
