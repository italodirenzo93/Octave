#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace graphics {

class IndexBuffer {
    friend class Mesh;
    friend class Renderer;

public:
    IndexBuffer() noexcept;
    IndexBuffer( const IndexBuffer& other ) noexcept;
    IndexBuffer( IndexBuffer&& other ) noexcept;
    ~IndexBuffer() noexcept;

    uint32_t GetElementCount() const noexcept { return element_count_; }

	std::vector<uint32_t> GetData() const noexcept;

    void SetData(std::initializer_list<uint32_t> initializerList) noexcept;
    void SetData(const std::vector<uint32_t>& indices) noexcept;

private:
    uint32_t id_ = 0;
    uint32_t element_count_ = 0;

public:
    IndexBuffer& operator=( const IndexBuffer& other ) noexcept;
    IndexBuffer& operator=( IndexBuffer&& other ) noexcept;
};

}  // namespace graphics

#endif
