#ifndef OCTAVE_GAMEPAD_HPP
#define OCTAVE_GAMEPAD_HPP

#include <string>

namespace Octave {

enum class GamepadButton {
	A,
	B,
	X,
	Y,
	LB,
	RB,
	Up,
	Down,
	Left,
	Right,
	Start,
	Select,
	LeftStick,
	RightStick
};

class Gamepad {
public:
	virtual ~Gamepad() noexcept = default;

	[[nodiscard]] virtual std::string GetName() const noexcept = 0;
	[[nodiscard]] virtual std::pair<float, float> GetLeftStick()
		const noexcept = 0;
	[[nodiscard]] virtual std::pair<float, float> GetRightStick()
		const noexcept = 0;
	[[nodiscard]] virtual bool IsButtonDown(
		GamepadButton button ) const noexcept = 0;
};

}  // namespace Octave

#endif  // OCTAVE_GAMEPAD_HPP
