#ifndef OCTAVE_GLFWERROR_HPP
#define OCTAVE_GLFWERROR_HPP

#include "Exception.hpp"

namespace octave::platform::glfw {

class GlfwError : public Exception {
public:
	GlfwError() noexcept;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_GLFWERROR_HPP
