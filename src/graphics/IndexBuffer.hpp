#ifndef OCTAVE_INDEXBUFFER_HPP
#define OCTAVE_INDEXBUFFER_HPP

#include <vector>

namespace Octave {

class IndexBuffer {
	friend class Mesh;

public:
	IndexBuffer() noexcept = default;
	IndexBuffer( const IndexBuffer& other ) = default;
	IndexBuffer( IndexBuffer&& other ) noexcept = default;
	virtual ~IndexBuffer() noexcept = default;

	IndexBuffer& operator=( const IndexBuffer& other ) = default;
	IndexBuffer& operator=( IndexBuffer&& other ) noexcept = default;

	[[nodiscard]] virtual uint32_t GetElementCount() const noexcept = 0;
	[[nodiscard]] virtual std::vector<uint32_t> GetData() const noexcept = 0;

	virtual void SetData( const std::vector<uint32_t>& indices ) noexcept = 0;
};

}  // namespace Octave

#endif
