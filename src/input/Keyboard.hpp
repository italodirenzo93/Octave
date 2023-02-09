#ifndef OCTAVE_KEYBOARD_HPP
#define OCTAVE_KEYBOARD_HPP

#include "graphics/Window.hpp"

namespace octave::input {

class Keyboard {
public:
	explicit Keyboard( const graphics::Window& window ) noexcept;
	~Keyboard() = default;

	[[nodiscard]] bool IsKeyDown( int key_code ) const noexcept;

private:
	const graphics::Window& window_;

	NON_COPYABLE_OR_MOVABLE_CLASS( Keyboard )
};

}  // namespace octave::input

#endif  // OCTAVE_KEYBOARD_HPP
