#ifndef OCTAVE_PLATFORMGLFW_HPP
#define OCTAVE_PLATFORMGLFW_HPP

#include "platform/Platform.hpp"

namespace Octave::Impl {

class PlatformGLFW : public Platform {
public:
	PlatformGLFW();
	~PlatformGLFW() noexcept override;

	[[nodiscard]] PlatformName GetName() const override;
	double GetElapsedTime() override;
	uint64_t GetPerformanceCounter() override;
	uint64_t GetPerformanceFrequency() override;

	void PollEvents() noexcept override;

	std::unique_ptr<Window> CreateWindow(
		const WindowOptions& options ) override;
};

}  // namespace Octave::Impl

#endif  // OCTAVE_PLATFORMGLFW_HPP
