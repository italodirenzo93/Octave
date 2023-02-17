#ifndef OCTAVE_PLATFORMGLFW_HPP
#define OCTAVE_PLATFORMGLFW_HPP

#include "platform/Platform.hpp"

namespace octave::platform::glfw {

class PlatformGlfw : public Platform {
public:
	PlatformGlfw();
	~PlatformGlfw() noexcept override;

	[[nodiscard]] std::string GetName() override;
	double GetElapsedTime() override;
	uint64_t GetPerformanceCounter() override;
	uint64_t GetPerformanceFrequency() override;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_PLATFORMGLFW_HPP
