#include "Keyboard.hpp"

namespace octave::input {

Keyboard::Keyboard( const graphics::Window& window ) noexcept
	: window_( window ) {
}

[[nodiscard]] bool Keyboard::IsKeyDown( int glfwKey ) const noexcept {
	return glfwGetKey( window_.handle_, glfwKey ) == GLFW_PRESS;
}

}  // namespace octave::input