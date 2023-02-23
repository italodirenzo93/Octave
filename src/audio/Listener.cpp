#include "Listener.hpp"

namespace octave::audio {

float Listener::GetGain() noexcept {
	float gain;
	alGetListenerf( AL_GAIN, &gain );
	return gain;
}

void Listener::SetGain( float gain ) {
	alListenerf( AL_GAIN, glm::clamp( gain, 0.0f, 1.0f ) );
	al::ThrowIfFailed();
}

glm::vec3 Listener::GetPosition() noexcept {
	float x, y, z;
	alGetListener3f( AL_POSITION, &x, &y, &z );
	return { x, y, z };
}

void Listener::SetPosition( const glm::vec3& position ) {
	alListenerfv( AL_POSITION, glm::value_ptr( position ) );
	al::ThrowIfFailed();
}

void Listener::SetPosition( float x, float y, float z ) {
	alListener3f( AL_POSITION, x, y, z );
	al::ThrowIfFailed();
}

glm::vec3 Listener::GetVelocity() noexcept {
	float x, y, z;
	alGetListener3f( AL_VELOCITY, &x, &y, &z );
	return { x, y, z };
}

void Listener::SetVelocity( const glm::vec3& velocity ) {
	alListenerfv( AL_VELOCITY, glm::value_ptr( velocity ) );
	al::ThrowIfFailed();
}

void Listener::SetVelocity( float x, float y, float z ) {
	alListener3f( AL_VELOCITY, x, y, z );
	al::ThrowIfFailed();
}

void Listener::SetOrientation( const glm::vec3& direction,
							   const glm::vec3& up ) {
	const ALfloat orientation[] = { direction.x, direction.y, direction.z,
									up.x,        up.y,        up.z };

	alListenerfv( AL_ORIENTATION, orientation );
	al::ThrowIfFailed();
}

}  // namespace octave::audio
