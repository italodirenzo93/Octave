#ifndef OCTAVE_SHADER_HPP
#define OCTAVE_SHADER_HPP

#include <filesystem>
#include <string>

namespace Octave {

class Shader {
public:
	virtual ~Shader() noexcept = default;

	/**
	 * Read and compile a shader program from disk
	 * @param vertex_path Path to vertex shader source file
	 * @param fragment_path Path to fragment shader source file
	 * @throws Shader::CompileError
	 */
	// void CompileFromFile( const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path );

	virtual void CompileFromString( const std::string& shader_source ) = 0;
};

}  // namespace Octave

#endif
