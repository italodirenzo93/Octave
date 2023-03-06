#ifndef OCTAVE_GRAPHICS_RESOURCES_HPP
#define OCTAVE_GRAPHICS_RESOURCES_HPP

#ifdef OGT_RHI_OPENGL4

#include "OpenGL/Buffer.hpp"
#include "OpenGL/Pipeline.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/VertexArray.hpp"

namespace Octave {
class Texture {};
}

#else
#error Unsupported RHI
#endif


#endif
