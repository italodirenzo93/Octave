#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "CommonInclude.hpp"
#include "VertexBuffer.hpp"

namespace graphics {
bool Initialize();
void Terminate();
void ResizeFramebuffer( int width, int height );
bool IsWindowOpen();
std::string GetRendererInfo();

void Clear( bool depth = true, float r = 0.0f, float g = 0.0f, float b = 0.0f,
            float a = 1.0f );
void Present();

void DrawPrimitives( const VertexArrayLayout& vao, const VertexBuffer& vbo );
}

#endif
