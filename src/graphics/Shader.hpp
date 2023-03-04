#ifndef OCTAVE_SHADER_HPP
#define OCTAVE_SHADER_HPP

#include <filesystem>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Exception.hpp"
#include "Texture.hpp"

namespace Octave {

namespace Impl {
	class GraphicsContextGL;
}

class Shader {
	friend class Impl::GraphicsContextGL;

public:
	class CompileError : public Exception {
		friend class Shader;
		explicit CompileError( const std::string& message ) noexcept
			: Exception( message ) {}
	};

public:
	Shader() noexcept = default;
	Shader( Shader&& other ) noexcept;
	~Shader() noexcept;

	Shader& SetBool( const std::string& name, bool value ) noexcept;
	Shader& SetInt( const std::string& name, int value ) noexcept;
	Shader& SetFloat( const std::string& name, float value ) noexcept;
	Shader& SetMat3( const std::string& name, const glm::mat3& value, bool transpose = false ) noexcept;
	Shader& SetMat4( const std::string& name, const glm::mat4& value, bool transpose = false ) noexcept;
	Shader& SetVec3( const std::string& name, const glm::vec3& value ) noexcept;
	Shader& SetVec3( const std::string& name, float x, float y, float z ) noexcept;
	Shader& SetTexture( const std::string& name, int index,
					 const Texture& texture ) noexcept;

	/**
	 * Read and compile a shader program from disk
	 * @param vertex_path Path to vertex shader source file
	 * @param fragment_path Path to fragment shader source file
	 * @throws Shader::CompileError
	 */
	void CompileFromFile( const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path );

	void CompileFromString( const std::string& vertex_source, const std::string& fragment_source );

private:
	uint32_t id_ = 0;

public:
	Shader& operator=( Shader&& other ) noexcept;

private:
	std::unordered_map<std::string, int> uniform_locations_;

	int GetUniform( const std::string& name ) noexcept;

public:
	Shader( const Shader& ) = delete;
	Shader& operator=( const Shader& ) = delete;
};

}  // namespace Octave

#endif
