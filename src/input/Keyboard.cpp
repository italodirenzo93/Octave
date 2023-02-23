#include "Keyboard.hpp"

#include "platform/glfw/WindowGLFW.hpp"
#include <GLFW/glfw3.h>

namespace Octave {

Keyboard::Keyboard( const Window& window ) noexcept : window_( window ) {
}

bool Keyboard::IsKeyDown( int key_code ) const noexcept {
	// TODO: This is a nasty hack to temporarily get the keyboard working again. Abstract this later.
	const auto& glfw_window = dynamic_cast<const Impl::WindowGLFW&>( window_ );
	return glfwGetKey( glfw_window.GetPointer(), key_code ) == GLFW_PRESS;
}

}  // namespace Octave
