#include "VertexArrayLayout.hpp"

namespace graphics {
using namespace std;

// NOTE: This must be kept in sync with the input layout of the vertex shader!
const map<LayoutSemantic, uint32_t> VertexArrayLayout::bindings_{
    {POSITION, 0}, {COLOR, 1}, {TEXCOORD, 2}, {NORMAL, 3}};

VertexArrayLayout::VertexArrayLayout() {
    glGenVertexArrays( 1, &vao_ );
}

VertexArrayLayout::VertexArrayLayout( const VertexArrayLayout& other ) {
    glGenVertexArrays( 1, &vao_ );

    layout_bindings_ = other.layout_bindings_;
}

VertexArrayLayout::VertexArrayLayout( VertexArrayLayout&& other ) noexcept {
    vao_ = other.vao_;
    layout_bindings_ = std::move( other.layout_bindings_ );

    other.vao_ = 0;
}

VertexArrayLayout::VertexArrayLayout(
    std::initializer_list<LayoutBinding> initializer )
    : layout_bindings_( initializer.begin(), initializer.end() ) {
    glGenVertexArrays( 1, &vao_ );
}

VertexArrayLayout::~VertexArrayLayout() {
    glDeleteVertexArrays( 1, &vao_ );
}

VertexArrayLayout& VertexArrayLayout::AddBinding(
    const LayoutBinding& binding ) {
    auto iter =
        std::find( layout_bindings_.begin(), layout_bindings_.end(), binding );
    if (iter != layout_bindings_.end()) {
        layout_bindings_.emplace( iter, binding );
    } else {
        layout_bindings_.emplace_back( binding );
    }

    return *this;
}

VertexArrayLayout& VertexArrayLayout::operator=(
    const VertexArrayLayout& other ) {
    glGenVertexArrays( 1, &vao_ );

    layout_bindings_ = other.layout_bindings_;

    return *this;
}

VertexArrayLayout& VertexArrayLayout::operator=(
    VertexArrayLayout&& other ) noexcept {
    vao_ = other.vao_;
    layout_bindings_ = std::move( other.layout_bindings_ );

    other.vao_ = 0;

    return *this;
}
} // namespace graphics
