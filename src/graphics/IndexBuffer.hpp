#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace graphics {

class IndexBuffer {
    friend class Mesh;
    friend class Renderer;

public:
    IndexBuffer();
    IndexBuffer( const IndexBuffer& other );
    IndexBuffer( IndexBuffer&& other ) noexcept;
    ~IndexBuffer();

    uint32_t GetElementCount() const { return element_count_; }

	std::vector<uint32_t> GetData() const;

    void SetData(std::initializer_list<uint32_t> initializerList);
    void SetData(const std::vector<uint32_t>& indices);

private:
    uint32_t id_ = 0;
    uint32_t element_count_ = 0;

public:
    IndexBuffer& operator=( const IndexBuffer& other );
    IndexBuffer& operator=( IndexBuffer&& other ) noexcept;
};

}  // namespace graphics

#endif
