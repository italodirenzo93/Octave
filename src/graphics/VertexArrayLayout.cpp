#include "VertexArrayLayout.hpp"

namespace graphics {
using namespace std;

// NOTE: This must be kept in sync with the input layout of the vertex shader!
const map<LayoutSemantic, uint32_t> VertexArrayLayout::Bindings{
    {POSITION, 0}, {COLOR, 1}, {TEXCOORD, 2}, {NORMAL, 3}};

VertexArrayLayout::VertexArrayLayout() {
    glGenVertexArrays( 1, &m_vertexArrayID );
}

VertexArrayLayout::VertexArrayLayout( const VertexArrayLayout& other ) {
    glGenVertexArrays( 1, &m_vertexArrayID );

    m_layoutBindings = other.m_layoutBindings;
}

VertexArrayLayout::VertexArrayLayout( VertexArrayLayout&& other ) noexcept {
    m_vertexArrayID = other.m_vertexArrayID;
    m_layoutBindings = std::move( other.m_layoutBindings );

    other.m_vertexArrayID = 0;
}

VertexArrayLayout::VertexArrayLayout(
    std::initializer_list<LayoutBinding> initializer )
    : m_layoutBindings( initializer.begin(), initializer.end() ) {
    glGenVertexArrays( 1, &m_vertexArrayID );
}

VertexArrayLayout::~VertexArrayLayout() {
    glDeleteVertexArrays( 1, &m_vertexArrayID );
}

VertexArrayLayout& VertexArrayLayout::addBinding(
    const LayoutBinding& binding ) {
    auto iter =
        std::find( m_layoutBindings.begin(), m_layoutBindings.end(), binding );
    if (iter != m_layoutBindings.end()) {
        m_layoutBindings.emplace( iter, binding );
    } else {
        m_layoutBindings.emplace_back( binding );
    }

    return *this;
}

VertexArrayLayout& VertexArrayLayout::operator=(
    const VertexArrayLayout& other ) {
    glGenVertexArrays( 1, &m_vertexArrayID );

    m_layoutBindings = other.m_layoutBindings;

    return *this;
}

VertexArrayLayout& VertexArrayLayout::operator=(
    VertexArrayLayout&& other ) noexcept {
    m_vertexArrayID = other.m_vertexArrayID;
    m_layoutBindings = std::move( other.m_layoutBindings );

    other.m_vertexArrayID = 0;

    return *this;
}
} // namespace graphics
