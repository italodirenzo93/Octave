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

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

// clang-format on

// C++ headers
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
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

namespace octave::graphics::gl {

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

}  // namespace octave::graphics::gl

namespace octave::audio::al {

class OpenALError : public Exception {
public:
	explicit OpenALError( ALenum error ) noexcept {
		switch ( error ) {
			case AL_INVALID_NAME:
				message_ = "AL_INVALID_NAME";
				break;
			case AL_INVALID_ENUM:
				message_ = "AL_INVALID_ENUM";
				break;
			case AL_INVALID_VALUE:
				message_ = "AL_INVALID_VALUE";
				break;
			case AL_INVALID_OPERATION:
				message_ = "AL_INVALID_OPERATION";
				break;
			case AL_OUT_OF_MEMORY:
				message_ = "AL_OUT_OF_MEMORY";
				break;
			default:
				message_ = "Unknown error";
				break;
		}
	}
};

inline void ThrowIfFailed() {
	const ALenum error = alGetError();
	if ( error != AL_NO_ERROR ) {
		throw OpenALError( error );
	}
}

}  // namespace octave::audio::al

#endif
