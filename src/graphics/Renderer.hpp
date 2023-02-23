#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "CommonInclude.hpp"

namespace graphics {
bool Initialize();

void Terminate();

void ResizeFramebuffer( int width, int height );

bool IsWindowOpen();

void Clear( bool depth = true, float r = 0.0f, float g = 0.0f, float b = 0.0f,
            float a = 1.0f );

void Present();

std::string GetRendererInfo();

}

#endif
