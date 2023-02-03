#include "Context.hpp"

#include "Exception.hpp"

#include <sstream>

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

	alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f );
	al::ThrowIfFailed();

	alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	al::ThrowIfFailed();

	const ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv( AL_ORIENTATION, orientation );
	al::ThrowIfFailed();

	alListenerf( AL_GAIN, 1.0f );
}

Context::~Context() noexcept {
	alcDestroyContext( context_ );
	alcCloseDevice( device_ );
}

std::string Context::GetDeviceInfo() const noexcept {
	std::ostringstream oss;

	oss << "Name: " << alcGetString(NULL, ALC_DEVICE_SPECIFIER) << std::endl;

	return oss.str();
}

}  // namespace octave::audio
