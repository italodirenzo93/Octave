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

    static uint32_t getSizeOfType( uint32_t type ) {
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

    void bind() const {
        glBindVertexArray( m_vertexArrayID );
    }

    void unbind() const {
        glBindVertexArray( 0 );
    }

private:
    uint32_t m_vertexArrayID;
    std::vector<LayoutBinding> m_layoutBindings;

public:
    void clearBindings() { m_layoutBindings.clear(); }
    VertexArrayLayout& addBinding( const LayoutBinding& binding );

    template <typename Vertex, uint32_t Stride = 1>
    void mapToBuffer( const VertexBuffer<Vertex>& vertexBuffer ) const {
        vertexBuffer.bind();
        bind();

        size_t offsetInBytes = 0;

        for (auto& binding : m_layoutBindings) {
            auto iter = Bindings.find( binding.semantic );
            if (iter == Bindings.end()) continue;

            const unsigned int bindingPoint = iter->second;

            glVertexAttribPointer(
                bindingPoint, static_cast<int>(binding.size), binding.type,
                binding.normalized, Stride * sizeof( Vertex ),
                reinterpret_cast<const void*>(offsetInBytes) );
            glEnableVertexAttribArray( bindingPoint );

            offsetInBytes +=
                binding.size * LayoutBinding::getSizeOfType( binding.type );
        }
    }

public:
    VertexArrayLayout& operator=( const VertexArrayLayout& other );
    VertexArrayLayout& operator=( VertexArrayLayout&& other ) noexcept;

private:
    static const std::map<LayoutSemantic, unsigned int> Bindings;
};
}

#endif
