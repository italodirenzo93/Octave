#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace graphics {

class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer( const IndexBuffer& other );
    IndexBuffer( IndexBuffer&& other ) noexcept;
    ~IndexBuffer();

    uint32_t GetId() const { return id_; }
    uint32_t GetSize() const { return element_count_; }

    void SetData(std::initializer_list<uint32_t> initializerList);

private:
    uint32_t id_ = 0;
    uint32_t element_count_ = 0;

public:
    IndexBuffer& operator=( const IndexBuffer& other );
    IndexBuffer& operator=( IndexBuffer&& other ) noexcept;
};

}  // namespace graphics

#endif
