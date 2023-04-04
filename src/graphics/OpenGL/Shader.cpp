#include "pch/pch.hpp"
#include "Shader.hpp"

namespace Octave {

Shader::Shader( ShaderType type ) noexcept : handle_( 0 ) {
	switch ( type ) {
		default:
		case ShaderType::VertexShader:
			type_ = GL_VERTEX_SHADER;
			break;
		case ShaderType::FragmentShader:
			type_ = GL_FRAGMENT_SHADER;
			break;
	}
}

}  // namespace Octave
