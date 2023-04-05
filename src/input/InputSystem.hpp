#ifndef OCTAVE_INPUTSYSTEM_HPP
#define OCTAVE_INPUTSYSTEM_HPP

#include "pch/pch.hpp"

#include "Gamepad.hpp"
#include "core/Window.hpp"

namespace Octave {

enum class Key { Escape, W, A, S, D, Q, E, Up, Down, Left, Right };

class InputSystem {
public:
	virtual ~InputSystem() noexcept = default;

	[[nodiscard]] virtual bool IsKeyDown( const Window& window,
										  Key key ) const noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<Gamepad> GetGamepad(
		int player_index ) const noexcept = 0;
};

}  // namespace Octave

#endif  // OCTAVE_INPUTSYSTEM_HPP
