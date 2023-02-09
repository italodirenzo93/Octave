#ifndef OCTAVE_GAMEPAD_HPP
#define OCTAVE_GAMEPAD_HPP

#include "graphics/Window.hpp"

namespace octave::input {

class Gamepad {
public:
	explicit Gamepad( int player_index ) noexcept;
	~Gamepad() noexcept = default;

	[[nodiscard]] std::string GetName() const noexcept;
	[[nodiscard]] std::tuple<float, float> GetLeftStick() const noexcept;
	[[nodiscard]] std::tuple<float, float> GetRightStick() const noexcept;
	[[nodiscard]] bool IsButtonDown( int button ) const noexcept;

	[[nodiscard]] static bool IsPresent( int player_index ) noexcept;

private:
	int player_index_;

public:
	Gamepad( const Gamepad& ) = delete;
	Gamepad& operator=( const Gamepad& ) = delete;
};

}  // namespace octave::input

#endif  // OCTAVE_GAMEPAD_HPP
