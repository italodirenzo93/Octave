#include "InputSystemGLFW.hpp"

#include "pch/pch.hpp"

#include <GLFW/glfw3.h>

#include "GamepadGLFW.hpp"
#include "core/glfw/WindowGLFW.hpp"

namespace Octave::Impl {

static int MapKey( Key key ) {
	switch ( key ) {
		case Key::W:
			return GLFW_KEY_W;
		case Key::S:
			return GLFW_KEY_S;
		case Key::A:
			return GLFW_KEY_A;
		case Key::D:
			return GLFW_KEY_D;
		case Key::Q:
			return GLFW_KEY_Q;
		case Key::E:
			return GLFW_KEY_E;
		case Key::Up:
			return GLFW_KEY_UP;
		case Key::Down:
			return GLFW_KEY_DOWN;
		case Key::Left:
			return GLFW_KEY_LEFT;
		case Key::Right:
			return GLFW_KEY_RIGHT;
		case Key::Escape:
			return GLFW_KEY_ESCAPE;
		default:
			return GLFW_KEY_UNKNOWN;
	}
}

bool InputSystemGLFW::IsKeyDown( const Window& window,
								 Key key ) const noexcept {
	const auto& glfw_window = dynamic_cast<const WindowGLFW&>( window );
	return glfwGetKey( glfw_window.GetPointer(), MapKey( key ) ) == GLFW_TRUE;
}

std::unique_ptr<Gamepad> InputSystemGLFW::GetGamepad(
	int player_index ) const noexcept {
	assert( player_index >= 0 );

	const int jid = GLFW_JOYSTICK_1 + player_index;
	if ( !glfwJoystickPresent( jid ) ) {
		return nullptr;
	}

	if ( !glfwJoystickIsGamepad( jid ) ) {
		std::cout << "Joystick is not an SDL compatible gamepad" << std::endl;
		return nullptr;
	}

	return std::make_unique<GamepadGLFW>( jid );
}

}  // namespace Octave::Impl
