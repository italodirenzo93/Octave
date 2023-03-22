#ifndef OCTAVE_GRAPHICS_OPENGL_BUFFER_HPP
#define OCTAVE_GRAPHICS_OPENGL_BUFFER_HPP

#include "graphics/Structs.hpp"
#include <glad/glad.h>

namespace Octave {

class Buffer {
public:
	Buffer( const BufferDescription& desc, const void* initial_data );
	~Buffer() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	[[nodiscard]] uint32_t GetSize() const noexcept;
	[[nodiscard]] uint32_t GetStride() const noexcept;
	[[nodiscard]] uint32_t GetNumElements() const noexcept;

	void SetData( const void* data, uint32_t size );

private:
	GLuint id_;
	BufferDescription desc_;
};

}

#endif
