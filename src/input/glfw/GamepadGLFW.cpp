#include "GamepadGLFW.hpp"

#include <GLFW/glfw3.h>

namespace Octave::Impl {

static int MapButton( GamepadButton button ) {
	switch ( button ) {
		case GamepadButton::A:
			return GLFW_GAMEPAD_BUTTON_A;
		case GamepadButton::B:
			return GLFW_GAMEPAD_BUTTON_B;
		case GamepadButton::X:
			return GLFW_GAMEPAD_BUTTON_X;
		case GamepadButton::Y:
			return GLFW_GAMEPAD_BUTTON_Y;
		case GamepadButton::LB:
			return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
		case GamepadButton::RB:
			return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
		case GamepadButton::LeftStick:
			return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
		case GamepadButton::RightStick:
			return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
		case GamepadButton::Up:
			return GLFW_GAMEPAD_BUTTON_DPAD_UP;
		case GamepadButton::Down:
			return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
		case GamepadButton::Left:
			return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
		case GamepadButton::Right:
			return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
		case GamepadButton::Start:
			return GLFW_GAMEPAD_BUTTON_START;
		case GamepadButton::Select:
			return GLFW_GAMEPAD_BUTTON_BACK;
		default:
			assert( false );
	}
}

GamepadGLFW::GamepadGLFW( int player_index ) noexcept {
	assert( player_index >= 0 );
	jid_ = player_index;
}

std::string GamepadGLFW::GetName() const noexcept {
	return glfwGetJoystickName( jid_ );
}

std::pair<float, float> GamepadGLFW::GetLeftStick() const noexcept {
	GLFWgamepadstate state = {};
	if ( !glfwGetGamepadState( jid_, &state ) ) {
		return { 0.0f, 0.0f };
	}
	return { state.axes[GLFW_GAMEPAD_AXIS_LEFT_X],
			 state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
}

std::pair<float, float> GamepadGLFW::GetRightStick() const noexcept {
	GLFWgamepadstate state = {};
	if ( !glfwGetGamepadState( jid_, &state ) ) {
		return { 0.0f, 0.0f };
	}
	return { state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X],
			 state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] };
}

bool GamepadGLFW::IsButtonDown( GamepadButton button ) const noexcept {
	GLFWgamepadstate state = {};
	if ( !glfwGetGamepadState( jid_, &state ) ) {
		return false;
	}
	return state.buttons[MapButton( button )];
}

}  // namespace Octave::Impl
