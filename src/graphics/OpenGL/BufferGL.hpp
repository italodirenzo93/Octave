#ifndef OCTAVE_GRAPHICS_OPENGL_BUFFERGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_BUFFERGL_HPP

#include <cstdint>
#include <cstddef>

#include "graphics/Buffer.hpp"

namespace Octave::Impl {

class BufferGL : public Buffer {
public:
	BufferGL() = delete;
	BufferGL( BufferBinding binding, size_t byteWidth );
	~BufferGL() noexcept override;

	[[nodiscard]] uint32_t GetId() const noexcept { return id_; }

	void SetData( const void* data, size_t size ) override;

protected:
	void Bind() const noexcept;
	void Unbind() const noexcept;

	uint32_t id_;
	BufferBinding binding_;
	size_t width_;
};

}

#endif
