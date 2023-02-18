#ifndef OCTAVE_INDEXBUFFER_HPP
#define OCTAVE_INDEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace Octave::graphics {

class IndexBuffer {
	friend class Mesh;
	friend class Renderer;

public:
	IndexBuffer() noexcept;
	IndexBuffer( const IndexBuffer& other ) noexcept;
	IndexBuffer( IndexBuffer&& other ) noexcept;
	~IndexBuffer() noexcept;

	[[nodiscard]] uint32_t GetElementCount() const noexcept {
		return element_count_;
	}

	[[nodiscard]] std::vector<uint32_t> GetData() const noexcept;

	void SetData( std::initializer_list<uint32_t> initializerList ) noexcept;
	void SetData( const std::vector<uint32_t>& indices ) noexcept;

private:
	uint32_t id_ = 0;
	uint32_t element_count_ = 0;

public:
	IndexBuffer& operator=( const IndexBuffer& other ) noexcept;
	IndexBuffer& operator=( IndexBuffer&& other ) noexcept;

	bool operator==( const IndexBuffer& other ) const noexcept;
};

}  // namespace octave::graphics

#endif
