#include "Keyboard.hpp"

namespace octave::input {

Keyboard::Keyboard( const graphics::Window& window ) noexcept
	: window_( window ) {
}

bool Keyboard::IsKeyDown( int key_code ) const noexcept {
	return glfwGetKey( window_.handle_, key_code ) == GLFW_PRESS;
}

}  // namespace octave::input
