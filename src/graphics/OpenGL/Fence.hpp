#ifndef OCTAVE_GRAPHICS_OPENGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_HPP

#include <glad/glad.h>

namespace Octave {

class Fence {
	friend class GraphicsContext;

public:
	Fence() noexcept : sync_( nullptr ) {}
	~Fence() noexcept {
		if ( sync_) {
			glDeleteSync( sync_ );
		}
	}

	Fence( const Fence& ) = delete;
	Fence& operator=( const Fence& ) = delete;

	[[nodiscard]] GLsync GetApiResource() const noexcept { return sync_; }

private:
	GLsync sync_;
};

}

#endif
