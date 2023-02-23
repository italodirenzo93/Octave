#ifndef OCTAVE_SHADER_HPP
#define OCTAVE_SHADER_HPP

#include "CommonInclude.hpp"
#include "Exception.hpp"
#include "Texture.hpp"

namespace octave::graphics {
class Shader {
	friend class Renderer;

public:
	class CompileError : public Exception {
		friend class Shader;
		explicit CompileError( const std::string& message ) noexcept
			: Exception( message ) {}
	};

public:
	/**
	 * Read and compile a shader program from disk
	 * @param vertex_path Path to vertex shader source file
	 * @param fragment_path Path to fragment shader source file
	 * @throws Shader::CompileError
	 */
	explicit Shader( const char* vertex_path, const char* fragment_path );
	Shader( Shader&& other ) noexcept;
	~Shader() noexcept;

	Shader& SetBool( const std::string& name, bool value ) noexcept;
	Shader& SetInt( const std::string& name, int value ) noexcept;
	Shader& SetFloat( const std::string& name, float value ) noexcept;
	Shader& SetMat4( const std::string& name, const glm::mat4& value ) noexcept;
	Shader& SetVec3( const std::string& name, const glm::vec3& value ) noexcept;
	Shader& SetVec3( const std::string& name, float x, float y, float z ) noexcept;
	Shader& SetTexture( const std::string& name, int index,
					 const Texture& texture ) noexcept;

private:
	uint32_t id_ = 0;

	Shader() = default;

public:
	Shader& operator=( Shader&& other ) noexcept;

private:
	std::unordered_map<std::string, int> uniform_locations_;

	int GetUniform( const std::string& name ) noexcept;

	static uint32_t CompileFromFile( const char* vertex_path,
									 const char* fragment_path );

	NON_COPYABLE_CLASS( Shader )
};
}  // namespace octave::graphics

#endif
