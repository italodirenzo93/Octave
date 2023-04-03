#ifndef OCTAVE_GRAPHICS_OPENGL_SHADER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADER_HPP

#include <array>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

class Shader {
public:
	Shader( ShaderType type, const std::string& source );
	~Shader() noexcept;

	Shader( const Shader& ) = delete;
	Shader& operator=( const Shader& ) = delete;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

private:
	GLuint id_;
};

}

#endif
