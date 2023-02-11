#include "Gamepad.hpp"

#include <GLFW/glfw3.h>

using namespace std;

namespace octave::input {

Gamepad::Gamepad( int player_index ) noexcept : player_index_( player_index ) {
	assert( glfwJoystickPresent( player_index ) );
	assert( glfwJoystickIsGamepad( player_index ) );
}

std::string Gamepad::GetName() const noexcept {
	return glfwGetGamepadName( player_index_ );
}

std::string Gamepad::GetGUID() const noexcept {
	return glfwGetJoystickGUID( player_index_ );
}

std::pair<float, float> Gamepad::GetLeftStick() const noexcept {
	GLFWgamepadstate state;
	if ( !glfwGetGamepadState( player_index_, &state ) ) {
		return make_tuple( 0.0f, 0.0f );
	}

	return { state.axes[GLFW_GAMEPAD_AXIS_LEFT_X],
			 state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
}

std::pair<float, float> Gamepad::GetRightStick() const noexcept {
	GLFWgamepadstate state;
	if ( !glfwGetGamepadState( player_index_, &state ) ) {
		return make_tuple( 0.0f, 0.0f );
	}

	return { state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X],
			 state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] };
}

bool Gamepad::IsButtonDown( int button ) const noexcept {
	GLFWgamepadstate state;
	if ( !glfwGetGamepadState( player_index_, &state ) ) {
		return false;
	}

	return state.buttons[button] == GLFW_PRESS;
}

bool Gamepad::IsPresent( int player_index ) noexcept {
	return glfwJoystickPresent( player_index ) &&
		   glfwJoystickIsGamepad( player_index );
}

}  // namespace octave::input
