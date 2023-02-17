#include "GlfwError.hpp"

#include <GLFW/glfw3.h>

namespace octave::platform::glfw {

GlfwError::GlfwError() noexcept {
	const char* description = nullptr;
	if ( glfwGetError( &description ) == GLFW_NO_ERROR ) {
		message_ = "Unknown GLFW error";
	} else {
		message_ = description;
	}
}

}  // namespace octave::platform::glfw
