#ifndef RENDERER_HPP
#define RENDERER_HPP

namespace graphics {
void Initialize();
void ResizeFramebuffer( int width, int height );
void Clear( bool depth = true, float r = 0.0f, float g = 0.0f, float b = 0.0f,
            float a = 1.0f );
}

#endif
