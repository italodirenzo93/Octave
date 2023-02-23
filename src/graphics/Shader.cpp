#include "Shader.hpp"

#include <fstream>
#include <sstream>

using namespace std;

namespace octave::graphics {

uint32_t Shader::CompileFromFile( const char* vertex_path,
								  const char* fragment_path ) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	// ensure ifstream objects can throw exceptions:
	v_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	f_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	try {
		// open files
		v_shader_file.open( vertex_path );
		f_shader_file.open( fragment_path );
		std::stringstream vShaderStream, f_shader_stream;
		// read file�s buffer contents into streams
		vShaderStream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = vShaderStream.str();
		fragment_code = f_shader_stream.str();
	} catch ( const std::ifstream::failure& ) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char info_log[512];
	// vertex Shader
	vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex, 1, &v_shader_code, nullptr );
	glCompileShader( vertex );
	// print compile errors if any
	glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		glGetShaderInfoLog( vertex, 512, nullptr, info_log );
		throw CompileError( info_log );
	}

	fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment, 1, &f_shader_code, nullptr );
	glCompileShader( fragment );
	// print compile errors if any
	glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		glGetShaderInfoLog( fragment, 512, nullptr, info_log );
		throw CompileError( info_log );
	}

	auto id = glCreateProgram();
	glAttachShader( id, vertex );
	glAttachShader( id, fragment );
	glLinkProgram( id );
	// print linking errors if any
	glGetProgramiv( id, GL_LINK_STATUS, &success );
	if ( !success ) {
		glGetProgramInfoLog( id, 512, nullptr, info_log );
		throw CompileError( info_log );
	}
	// delete shaders; they�re linked into our program and no longer necessary
	glDeleteShader( vertex );
	glDeleteShader( fragment );

	return id;
}

Shader::Shader( const char* vertex_path, const char* fragment_path ) {
	id_ = CompileFromFile( vertex_path, fragment_path );
}

Shader::Shader( Shader&& other ) noexcept : id_( other.id_ ) {
	other.id_ = 0;
}

Shader::~Shader() noexcept {
	glDeleteProgram( id_ );
}

Shader& Shader::SetBool( const std::string& name, bool value ) noexcept {
	glUniform1i( GetUniform( name ), static_cast<int>( value ) );
	return *this;
}

Shader& Shader::SetInt( const std::string& name, int value ) noexcept {
	glUniform1i( GetUniform( name ), value );
	return *this;
}

Shader& Shader::SetFloat( const std::string& name, float value ) noexcept {
	glUniform1f( GetUniform( name ), value );
	return *this;
}

Shader& Shader::SetMat4( const std::string& name,
					  const glm::mat4& value ) noexcept {
	glUniformMatrix4fv( GetUniform( name ), 1, GL_FALSE,
						glm::value_ptr( value ) );
	return *this;
}

Shader& Shader::SetVec3( const std::string& name,
					  const glm::vec3& value ) noexcept {
	glUniform3fv( GetUniform( name ), 1, glm::value_ptr( value ) );
	return *this;
}

Shader& Shader::SetVec3( const std::string& name, float x, float y,
					  float z ) noexcept {
	glUniform3f( GetUniform( name ), x, y, z );
	return *this;
}

Shader& Shader::SetTexture( const std::string& name, int index,
						 const Texture& texture ) noexcept {
	assert( index >= 0 );

	const auto uniform_name = name + "[" + to_string( index ) + "]";
	const auto location = GetUniform( uniform_name );
	glUniform1i( location, index );

	glActiveTexture( GL_TEXTURE0 + index );
	glBindTexture( GL_TEXTURE_2D, texture.id_ );

	return *this;
}

int Shader::GetUniform( const std::string& name ) noexcept {
	const auto iter = uniform_locations_.find( name );
	if ( iter == uniform_locations_.end() ) {
		const auto location = glGetUniformLocation( id_, name.c_str() );
		if ( location < 0 ) {
			return -1;
		}

		uniform_locations_[name] = location;

		return location;
	}

	return iter->second;
}

Shader& Shader::operator=( Shader&& other ) noexcept {
	id_ = other.id_;
	other.id_ = 0;

	return *this;
}

}  // namespace octave::graphics
