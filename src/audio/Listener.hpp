#ifndef OCTAVE_LISTENER_HPP
#define OCTAVE_LISTENER_HPP

#include "CommonInclude.hpp"

namespace Octave::audio {

class Listener final {
public:
	[[nodiscard]] static float GetGain() noexcept;
	static void SetGain( float gain );

	[[nodiscard]] static glm::vec3 GetPosition() noexcept;
	static void SetPosition( const glm::vec3& position );
	static void SetPosition( float x, float y, float z );

	[[nodiscard]] static glm::vec3 GetVelocity() noexcept;
	static void SetVelocity( const glm::vec3& velocity );
	static void SetVelocity( float x, float y, float z );

	static void SetOrientation( const glm::vec3& direction, const glm::vec3& up );
};

}  // namespace octave

#endif  // OCTAVE_LISTENER_HPP
