#ifndef OCTAVE_PLATFORM_HPP
#define OCTAVE_PLATFORM_HPP

#include <cstdint>
#include <string>

namespace octave::platform {

class Platform {
public:
	virtual ~Platform() = default;
	[[nodiscard]] virtual std::string GetName() = 0;
	virtual double GetElapsedTime() = 0;
	virtual uint64_t GetPerformanceCounter() = 0;
	virtual uint64_t GetPerformanceFrequency() = 0;
};

}

#endif  // OCTAVE_PLATFORM_HPP
