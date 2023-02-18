#ifndef OCTAVE_COMMONINCLUDE_HPP
#define OCTAVE_COMMONINCLUDE_HPP

// clang-format off

// Library headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// clang-format on

// C++ headers
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// C headers
#include <cstdint>

#include "Exception.hpp"

#define NON_COPYABLE_CLASS( className )     \
	className( const className& ) = delete; \
	className& operator=( const className& ) = delete;

#define NON_MOVABLE_CLASS( className ) \
	className( className&& ) = delete; \
	className& operator=( className&& ) = delete;

#define NON_COPYABLE_OR_MOVABLE_CLASS( className ) \
	NON_COPYABLE_CLASS( className )                \
	NON_MOVABLE_CLASS( className )

#define SELF_REFERENCE_CHECK( other ) \
	if ( &other == this ) return *this

namespace Octave::graphics::gl {

class OpenGLError : public Exception {
public:
	explicit OpenGLError( GLenum error ) noexcept {
		switch ( error ) {
			case GL_INVALID_ENUM:
				message_ = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				message_ = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				message_ = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				message_ = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				message_ = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				message_ = "GL_OUT_OF_MEMORY";
				break;
			default:
				message_ = "Unknown error";
				break;
		}
	}
};

inline void ThrowIfFailed() {
	const GLenum error = glGetError();
	if ( error != GL_NO_ERROR ) {
		throw OpenGLError( error );
	}
}

inline void CheckErrors( const char* file, int line ) {
	GLenum error = GL_NO_ERROR;
	while ( ( error = glGetError() ) ) {
		const char* message;

		switch ( error ) {
			case GL_INVALID_ENUM:
				message = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				message = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				message = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				message = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				message = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				message = "GL_OUT_OF_MEMORY";
				break;
			default:
				message = "Unknown error";
				break;
		}

		std::cerr << "OpenGL error:" << std::endl
				  << "---------------" << std::endl
				  << file << ":" << line << " - " << message << std::endl
				  << std::endl;
	}
}

#define GL_CALL( func ) \
	func;               \
	Octave::graphics::gl::CheckErrors( __FILE__, __LINE__ )

}  // namespace octave::graphics::gl

#endif
