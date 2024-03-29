#ifndef OCTAVE_GRAPHICS_OPENGL_BUFFER_HPP
#define OCTAVE_GRAPHICS_OPENGL_BUFFER_HPP

#include "graphics/Structs.hpp"
#include <glad/glad.h>

namespace Octave {

class Buffer {
public:
	Buffer( const BufferDescription& desc ) noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint handle ) noexcept { handle_ = handle; }

	[[nodiscard]] uint32_t GetSize() const noexcept;
	[[nodiscard]] uint32_t GetStride() const noexcept;
	[[nodiscard]] uint32_t GetNumElements() const noexcept;

	[[nodiscard]] GLenum GetBindTarget() const noexcept { return target_; }


	void SetData( const void* data, uint32_t offset, uint32_t size );

private:
	GLuint handle_;
	GLenum target_;
	BufferDescription desc_;
};

}  // namespace Octave

#endif
