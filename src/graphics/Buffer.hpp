#ifndef OCTAVE_GRAPHICS_BUFFER_HPP
#define OCTAVE_GRAPHICS_BUFFER_HPP

#include <cstddef>

namespace Octave {

enum class BufferBinding {
	VertexBuffer,
	IndexBuffer,
	ConstantBuffer
};

class Buffer {
public:
	virtual ~Buffer() noexcept = default;

	virtual void SetData( const void* data, size_t size ) = 0;
};

}

#endif
