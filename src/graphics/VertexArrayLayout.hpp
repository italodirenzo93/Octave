#ifndef VERTEX_ARRAY_LAYOUT_HPP
#define VERTEX_ARRAY_LAYOUT_HPP

#include "CommonInclude.hpp"

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
    friend class VertexBuffer;
    friend class Mesh;
    friend class Renderer;

public:
    VertexArrayLayout();
    VertexArrayLayout( const VertexArrayLayout& other );
    VertexArrayLayout( VertexArrayLayout&& other ) noexcept;
    VertexArrayLayout( std::initializer_list<LayoutBinding> initializer );
    ~VertexArrayLayout();

    void ClearBindings();
    VertexArrayLayout& AddBinding( const LayoutBinding& binding );
    VertexArrayLayout& AddBindings( std::initializer_list<LayoutBinding> initializerList );

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
