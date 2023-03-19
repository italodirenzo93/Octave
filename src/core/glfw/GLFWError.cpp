#include "GLFWError.hpp"

#include <GLFW/glfw3.h>

namespace Octave {

GLFWError::GLFWError() noexcept {
	const char* description = nullptr;
	if ( glfwGetError( &description ) == GLFW_NO_ERROR ) {
		message_ = "Unknown GLFW error";
	} else {
		message_ = description;
	}
}

}  // namespace Octave::Impl
