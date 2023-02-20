#ifndef OCTAVE_INPUTSYSTEMGLFW_HPP
#define OCTAVE_INPUTSYSTEMGLFW_HPP

#include "input/InputSystem.hpp"

namespace Octave::Impl {

class InputSystemGLFW : public InputSystem {
public:
	[[nodiscard]] bool IsKeyDown( const Window& window, Key key ) const noexcept override;

	[[nodiscard]] std::unique_ptr<Gamepad> GetGamepad(
		int player_index ) const noexcept override;
};

}  // namespace Octave::Impl

#endif  // OCTAVE_INPUTSYSTEMGLFW_HPP
