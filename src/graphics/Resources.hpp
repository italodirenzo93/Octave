#ifndef OCTAVE_GRAPHICS_RESOURCES_HPP
#define OCTAVE_GRAPHICS_RESOURCES_HPP

#ifdef OGT_RHI_OPENGL46

#include "OpenGL/Buffer.hpp"
#include "OpenGL/Fence.hpp"
#include "OpenGL/Pipeline.hpp"
#include "OpenGL/Sampler.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/VertexArray.hpp"
#include "OpenGL/Texture2D.hpp"

#else

#error Unsupported RHI

#endif


#endif
