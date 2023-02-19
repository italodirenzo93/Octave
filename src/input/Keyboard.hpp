#ifndef OCTAVE_KEYBOARD_HPP
#define OCTAVE_KEYBOARD_HPP

#include "platform/Window.hpp"

namespace Octave {

class Keyboard {
public:
	explicit Keyboard( const Window& window ) noexcept;
	~Keyboard() = default;

	[[nodiscard]] bool IsKeyDown( int key_code ) const noexcept;

private:
	const Window& window_;

	NON_COPYABLE_OR_MOVABLE_CLASS( Keyboard )
};

}  // namespace Octave

#endif  // OCTAVE_KEYBOARD_HPP
