#ifndef OCTAVE_AUDIO_SOURCE_HPP
#define OCTAVE_AUDIO_SOURCE_HPP

#include "Buffer.hpp"

namespace Octave::audio {

enum class PlaybackState {
	Initial,
	Playing,
	Paused,
	Stopped
};

class Source {
public:
	Source();
	~Source() noexcept;

	// Properties
	Source& SetBuffer( const Buffer& buffer );

	[[nodiscard]] float GetGain() const noexcept;
	Source& SetGain( float gain );

	[[nodiscard]] bool IsLooping() const noexcept;
	Source& SetLooping( bool loop );

	[[nodiscard]] float GetPitch() const noexcept;
	Source& SetPitch( float pitch );

	[[nodiscard]] glm::vec3 GetPosition() const noexcept;
	Source& SetPosition( const glm::vec3& position );
	Source& SetPosition( float x, float y, float z );

	[[nodiscard]] PlaybackState GetState() const noexcept;

	void Play();
	void Pause();
	void Stop();

private:
	uint32_t id_ = 0;
};

}  // namespace octave::audio

#endif  // OCTAVE_AUDIO_SOURCE_HPP
