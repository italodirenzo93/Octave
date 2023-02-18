#ifndef OCTAVE_ALCOMMON_HPP
#define OCTAVE_ALCOMMON_HPP

#include "Exception.hpp"

#ifndef AL_LIBTYPE_STATIC
#define AL_LIBTYPE_STATIC
#endif
#include <AL/al.h>

#include <iostream>

namespace Octave::audio::al {

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

inline void CheckErrors( const char* file, int line ) {
	ALenum error = AL_NO_ERROR;
	while ( ( error = alGetError() ) ) {
		const char* message;

		switch ( error ) {
			case AL_INVALID_NAME:
				message = "AL_INVALID_NAME";
				break;
			case AL_INVALID_ENUM:
				message = "AL_INVALID_ENUM";
				break;
			case AL_INVALID_VALUE:
				message = "AL_INVALID_VALUE";
				break;
			case AL_INVALID_OPERATION:
				message = "AL_INVALID_OPERATION";
				break;
			case AL_OUT_OF_MEMORY:
				message = "AL_OUT_OF_MEMORY";
				break;
			default:
				message = "Unknown error";
				break;
		}

		std::cerr << "OpenAL error:" << std::endl
				  << "---------------" << std::endl
				  << file << ":" << line << " - " << message << std::endl
				  << std::endl;
	}
}

#define AL_CALL( func ) \
	func;               \
	Octave::audio::al::CheckErrors( __FILE__, __LINE__ )

}  // namespace octave::audio::al

#endif  // OCTAVE_ALCOMMON_HPP
