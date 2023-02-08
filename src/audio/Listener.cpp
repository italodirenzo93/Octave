#include "Listener.hpp"

#include "alCommon.hpp"

namespace octave::audio {

float Listener::GetGain() noexcept {
	float gain;
	alGetListenerf( AL_GAIN, &gain );
	return gain;
}

void Listener::SetGain( float gain ) {
	AL_CALL( alListenerf( AL_GAIN, glm::clamp( gain, 0.0f, 1.0f ) ) );
}

glm::vec3 Listener::GetPosition() noexcept {
	float x, y, z;
	alGetListener3f( AL_POSITION, &x, &y, &z );
	return { x, y, z };
}

void Listener::SetPosition( const glm::vec3& position ) {
	AL_CALL( alListenerfv( AL_POSITION, glm::value_ptr( position ) ) );
}

void Listener::SetPosition( float x, float y, float z ) {
	AL_CALL( alListener3f( AL_POSITION, x, y, z ) );
}

glm::vec3 Listener::GetVelocity() noexcept {
	float x, y, z;
	alGetListener3f( AL_VELOCITY, &x, &y, &z );
	return { x, y, z };
}

void Listener::SetVelocity( const glm::vec3& velocity ) {
	AL_CALL( alListenerfv( AL_VELOCITY, glm::value_ptr( velocity ) ) );
}

void Listener::SetVelocity( float x, float y, float z ) {
	AL_CALL( alListener3f( AL_VELOCITY, x, y, z ) );
}

void Listener::SetOrientation( const glm::vec3& direction,
							   const glm::vec3& up ) {
	const ALfloat orientation[] = { direction.x, direction.y, direction.z,
									up.x,        up.y,        up.z };

	AL_CALL( alListenerfv( AL_ORIENTATION, orientation ) );
}

}  // namespace octave::audio
