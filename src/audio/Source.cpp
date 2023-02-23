#include "Source.hpp"

#include "Exception.hpp"

namespace octave::audio {

static inline void ThrowIfError() {
	const ALenum error = alGetError();
	if ( error != AL_NO_ERROR ) {
		throw Exception( "OpenAL error " + std::to_string( error ) );
	}
}

Source::Source() noexcept {
	alGenSources( 1, &id_ );
	ThrowIfError();
}

Source::~Source() noexcept {
	alDeleteSources( 1, &id_ );
}

Source& Source::SetBuffer( const Buffer& buffer ) {
	alSourcei( id_, AL_BUFFER, static_cast<int>( buffer.id_ ) );
	ThrowIfError();
	return *this;
}

Source& Source::SetGain( float gain ) {
	alSourcef( id_, AL_GAIN, std::max( gain, 0.0f ) );
	ThrowIfError();
	return *this;
}

bool Source::IsLooping() const noexcept {
	ALint looping;
	alGetSourcei( id_, AL_LOOPING, &looping );
	return static_cast<bool>( looping );
}

Source& Source::SetLooping( bool loop ) {
	alSourcei( id_, AL_LOOPING, loop );
	ThrowIfError();
	return *this;
}

glm::vec3 Source::GetPosition() const noexcept {
	glm::vec3 position;
	alGetSourcefv( id_, AL_POSITION, glm::value_ptr( position ) );
	return position;
}

float Source::GetPitch() const noexcept {
	float pitch;
	alGetSourcef( id_, AL_PITCH, &pitch );
	return pitch;
}

Source& Source::SetPitch( float pitch ) {
	alSourcef( id_, AL_PITCH, pitch );
	ThrowIfError();
	return *this;
}

Source& Source::SetPosition( const glm::vec3& position ) {
	alSourcefv( id_, AL_POSITION, glm::value_ptr( position ) );
	ThrowIfError();
	return *this;
}

Source& Source::SetPosition( float x, float y, float z ) {
	alSource3f( id_, AL_POSITION, x, y, z );
	ThrowIfError();
	return *this;
}

void Source::Play() {
	alSourcePlay( id_ );
	ThrowIfError();
}

}  // namespace octave::audio
