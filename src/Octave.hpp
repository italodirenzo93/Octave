#ifndef OCTAVE_OCTAVE_HPP
#define OCTAVE_OCTAVE_HPP

#include "graphics/IndexBuffer.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/Texture.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/Window.hpp"

#include "audio/Buffer.hpp"
#include "audio/Listener.hpp"
#include "audio/Source.hpp"

#include "input/Gamepad.hpp"
#include "input/Keyboard.hpp"

namespace octave {

bool Up(int argc, char* argv[]);
void Down();

}  // namespace octave

#endif  // OCTAVE_OCTAVE_HPP
