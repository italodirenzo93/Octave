#include "Source.hpp"

#include "Exception.hpp"

#include "alCommon.hpp"

namespace Octave::audio {

Source::Source() {
	alGenSources( 1, &id_ );
}

Source::~Source() noexcept {
	PlaybackState state = GetState();
	if ( state == PlaybackState::Playing || state == PlaybackState::Paused ) {
		Stop();
	}

	AL_CALL( alSourcei( id_, AL_BUFFER, 0 ) );
	alDeleteSources( 1, &id_ );
}

Source& Source::SetBuffer( const Buffer& buffer ) {
	AL_CALL( alSourcei( id_, AL_BUFFER, static_cast<int>( buffer.id_ ) ) );
	return *this;
}

float Source::GetGain() const noexcept {
	float gain;
	alGetSourcef( id_, AL_GAIN, &gain );
	return gain;
}

Source& Source::SetGain( float gain ) {
	AL_CALL( alSourcef( id_, AL_GAIN, std::max( gain, 0.0f ) ) );
	return *this;
}

bool Source::IsLooping() const noexcept {
	ALint looping;
	alGetSourcei( id_, AL_LOOPING, &looping );
	return static_cast<bool>( looping );
}

Source& Source::SetLooping( bool loop ) {
	AL_CALL( alSourcei( id_, AL_LOOPING, loop ) );
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
	AL_CALL( alSourcef( id_, AL_PITCH, std::max( pitch, 0.0f ) ) );
	return *this;
}

Source& Source::SetPosition( const glm::vec3& position ) {
	AL_CALL( alSourcefv( id_, AL_POSITION, glm::value_ptr( position ) ) );
	return *this;
}

Source& Source::SetPosition( float x, float y, float z ) {
	AL_CALL( alSource3f( id_, AL_POSITION, x, y, z ) );
	return *this;
}

PlaybackState Source::GetState() const noexcept {
	int state;
	alGetSourcei( id_, AL_SOURCE_STATE, &state );

	switch ( state ) {
		default:
		case AL_INITIAL:
			return PlaybackState::Initial;
		case AL_PLAYING:
			return PlaybackState::Playing;
		case AL_PAUSED:
			return PlaybackState::Paused;
		case AL_STOPPED:
			return PlaybackState::Stopped;
	}
}

void Source::Play() {
	AL_CALL( alSourcePlay( id_ ) );
}

void Source::Pause() {
	AL_CALL( alSourcePause( id_ ) );
}

void Source::Stop() {
	AL_CALL( alSourceStop( id_ ) );
}

}  // namespace octave::audio
