#ifndef OCTAVE_AUDIO_SOURCE_HPP
#define OCTAVE_AUDIO_SOURCE_HPP

#include "Buffer.hpp"
#include "CommonInclude.hpp"

namespace octave::audio {

class Source {
public:
	Source() noexcept;
	~Source() noexcept;

	// Properties
	Source& SetBuffer( const Buffer& buffer );
	Source& SetGain( float gain );

	[[nodiscard]] bool IsLooping() const noexcept;
	Source& SetLooping( bool loop );

	[[nodiscard]] float GetPitch() const noexcept;
	Source& SetPitch( float pitch );

	[[nodiscard]] glm::vec3 GetPosition() const noexcept;
	Source& SetPosition( const glm::vec3& position );
	Source& SetPosition( float x, float y, float z );

	void Play();

private:
	uint32_t id_ = 0;
};

}  // namespace octave::audio

#endif  // OCTAVE_AUDIO_SOURCE_HPP
