#ifndef OCTAVE_GRAPHICS_OPENGL_SHADERGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADERGL_HPP

#include <unordered_map>

#include "graphics/Shader.hpp"
#include "Exception.hpp"

namespace Octave::Impl {

class ShaderGL : public Shader {
public:
	class CompileError : public Exception {
		friend class ShaderGL;
		explicit CompileError( const std::string& message ) noexcept
			: Exception( message ) {}
	};

	ShaderGL();
	~ShaderGL() noexcept override;

	[[nodiscard]] uint32_t GetId() const noexcept { return id_; }

	void CompileFromString( const std::string& shader_source ) override;

private:
	// int GetUniform( const std::string& name ) noexcept;

	uint32_t id_ = 0;
	std::unordered_map<std::string, int> uniform_locations_;
};

}

#endif
