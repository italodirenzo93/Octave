#include <GLFW/glfw3.h>

#include "PlatformGlfw.hpp"
#include "GlfwError.hpp"

namespace octave::platform::glfw {

PlatformGlfw::PlatformGlfw() {
	if ( !glfwInit() ) {
		throw GlfwError();
	}
}

PlatformGlfw::~PlatformGlfw() noexcept {
	glfwTerminate();
}

double PlatformGlfw::GetElapsedTime() {
	return glfwGetTime();
}

uint64_t PlatformGlfw::GetPerformanceCounter() {
	return glfwGetTimerValue();
}

uint64_t PlatformGlfw::GetPerformanceFrequency() {
	return glfwGetTimerFrequency();
}
std::string PlatformGlfw::GetName() {
	return "GLFW";
}

}  // namespace octave::platform::glfw
