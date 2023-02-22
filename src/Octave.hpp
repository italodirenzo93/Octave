#ifndef OCTAVE_OCTAVE_HPP
#define OCTAVE_OCTAVE_HPP

// clang-format off

#include "Application.hpp"
#include "Exception.hpp"
#include "Config.hpp"
#include "Camera.hpp"

#include "platform/Platform.hpp"
#include "platform/Window.hpp"

#include "input/Gamepad.hpp"
#include "input/InputSystem.hpp"

#include "graphics/IndexBuffer.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"
#include "graphics/VertexBuffer.hpp"

#include "resources/ResourceManager.hpp"
#include "resources/ShaderManager.hpp"

#include "helpers/GeometricPrimitive.hpp"
#include "helpers/StepTimer.hpp"

#if defined( _WIN32 ) && defined( CreateWindow )
#undef CreateWindow
#endif

#endif  // OCTAVE_OCTAVE_HPP
