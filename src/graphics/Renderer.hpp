#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "CommonInclude.hpp"

namespace graphics {
void Initialize();
void ResizeFramebuffer( int width, int height );
void Clear( bool depth = true, float r = 0.0f, float g = 0.0f, float b = 0.0f,
            float a = 1.0f );
std::string GetRendererInfo();

}

#endif
