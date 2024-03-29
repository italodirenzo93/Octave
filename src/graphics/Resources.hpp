#ifndef OCTAVE_GRAPHICS_RESOURCES_HPP
#define OCTAVE_GRAPHICS_RESOURCES_HPP

#ifdef OGT_RHI_OPENGL

#include "OpenGL/Buffer.hpp"
#include "OpenGL/Program.hpp"
#include "OpenGL/Sampler.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/Texture2D.hpp"

#else

#error Unsupported RHI

#endif


#endif
