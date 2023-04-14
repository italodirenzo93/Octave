#ifndef OCTAVE_INPUT_NULL_INPUTSYSTEMNULL_HPP
#define OCTAVE_INPUT_NULL_INPUTSYSTEMNULL_HPP

#include "input/InputSystem.hpp"

namespace Octave {

class InputSystemNull : public InputSystem {
	[[nodiscard]] bool IsKeyDown( const Window& window,
										  Key key ) const noexcept override { return false; }

	[[nodiscard]] std::unique_ptr<Gamepad> GetGamepad(
		int player_index ) const noexcept override { return nullptr; }
};

};

#endif
