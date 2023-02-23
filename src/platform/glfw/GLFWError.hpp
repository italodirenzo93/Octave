#ifndef OCTAVE_GLFWERROR_HPP
#define OCTAVE_GLFWERROR_HPP

#include "Exception.hpp"

namespace octave::platform::glfw {

class GLFWError : public Exception {
public:
	GLFWError() noexcept;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_GLFWERROR_HPP