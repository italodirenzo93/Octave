#ifndef VERTEX_ARRAY_LAYOUT_HPP
#define VERTEX_ARRAY_LAYOUT_HPP

#include "CommonInclude.hpp"
#include "Buffer.hpp"

namespace graphics {
enum LayoutSemantic { POSITION, COLOR, TEXCOORD, NORMAL };

struct LayoutBinding {
    LayoutSemantic semantic;
    unsigned int size;
    unsigned int type;
    bool normalized;

    bool operator==( const LayoutBinding& other ) const {
        return semantic == other.semantic && size == other.size &&
               type == other.type && normalized == other.normalized;
    }

    static uint32_t GetSizeOfType( uint32_t type ) {
        switch (type) {
            case GL_FLOAT:
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            default:
                assert( false );
                return 0;
        }
    }
};

class VertexArrayLayout {
public:
    VertexArrayLayout();
    VertexArrayLayout( const VertexArrayLayout& other );
    VertexArrayLayout( VertexArrayLayout&& other ) noexcept;
    VertexArrayLayout( std::initializer_list<LayoutBinding> initializer );
    ~VertexArrayLayout();

    void ClearBindings();
    VertexArrayLayout& AddBinding( const LayoutBinding& binding );
    VertexArrayLayout& AddBindings( std::initializer_list<LayoutBinding> initializerList );

    template <typename Vertex, uint32_t Stride = 1>
    void MapToBuffer( const VertexBuffer<Vertex>& vertex_buffer ) const {
        vertex_buffer.Bind();
        glBindVertexArray( id_ );

        size_t offset_in_bytes = 0;

        for (auto& binding : layout_bindings_) {
            auto iter = bindings_.find( binding.semantic );
            if (iter == bindings_.end()) continue;

            const uint32_t binding_point = iter->second;

            glVertexAttribPointer(
                binding_point, static_cast<int>(binding.size), binding.type,
                binding.normalized, Stride * sizeof( Vertex ),
                reinterpret_cast<const void*>(offset_in_bytes) );
            glEnableVertexAttribArray( binding_point );

            offset_in_bytes +=
                binding.size * LayoutBinding::GetSizeOfType( binding.type );
        }
    }

private:
    uint32_t id_ = 0;
    std::vector<LayoutBinding> layout_bindings_;

public:
    VertexArrayLayout& operator=( const VertexArrayLayout& other );
    VertexArrayLayout& operator=( VertexArrayLayout&& other ) noexcept;

private:
    static const std::map<LayoutSemantic, unsigned int> bindings_;
};
}

#endif
