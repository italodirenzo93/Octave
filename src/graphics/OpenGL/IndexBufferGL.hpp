#ifndef OCTAVE_INDEXBUFFERGL_HPP
#define OCTAVE_INDEXBUFFERGL_HPP

#include "graphics/IndexBuffer.hpp"

namespace Octave::Impl {

class IndexBufferGL : public IndexBuffer {
public:
	IndexBufferGL();
	~IndexBufferGL() noexcept override;

	[[nodiscard]] uint32_t GetId() const noexcept { return id_; }
	[[nodiscard]] uint32_t GetElementCount() const noexcept override;
	[[nodiscard]] std::vector<uint32_t> GetData() const noexcept override;

	void SetData( const std::vector<uint32_t>& indices ) noexcept override;

private:
	uint32_t id_ = 0;
	uint32_t element_count_ = 0;
};

}

#endif
