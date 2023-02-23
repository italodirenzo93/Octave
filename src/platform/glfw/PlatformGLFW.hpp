#ifndef OCTAVE_PLATFORMGLFW_HPP
#define OCTAVE_PLATFORMGLFW_HPP

#include "platform/Platform.hpp"

namespace octave::platform::glfw {

class PlatformGLFW : public Platform {
public:
	PlatformGLFW();
	~PlatformGLFW() noexcept override;

	[[nodiscard]] PlatformName GetName() const override;
	double GetElapsedTime() override;
	uint64_t GetPerformanceCounter() override;
	uint64_t GetPerformanceFrequency() override;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_PLATFORMGLFW_HPP
