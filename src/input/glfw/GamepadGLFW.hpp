#ifndef OCTAVE_GAMEPADGLFW_HPP
#define OCTAVE_GAMEPADGLFW_HPP

#include "input/Gamepad.hpp"

namespace Octave::Impl {

class GamepadGLFW : public Gamepad {
public:
	explicit GamepadGLFW( int player_index ) noexcept;

	[[nodiscard]] std::string GetName() const noexcept override;
	[[nodiscard]] std::pair<float, float> GetLeftStick() const noexcept override;
	[[nodiscard]] std::pair<float, float> GetRightStick() const noexcept override;
	[[nodiscard]] bool IsButtonDown( GamepadButton button ) const noexcept override;

private:
	int jid_;
};

}  // namespace Octave

#endif  // OCTAVE_GAMEPADGLFW_HPP
