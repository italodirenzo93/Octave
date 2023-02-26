#include "pch/pch.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include <fstream>
#include <sstream>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_NONE
#include "Config.hpp"
#include "stb_include.h"

using namespace std;

namespace Octave {

Shader::Shader( Shader&& other ) noexcept : id_( other.id_ ) {
	other.id_ = 0;
}

Shader::~Shader() noexcept {
	glDeleteProgram( id_ );
}

Shader& Shader::SetBool( const std::string& name, bool value ) noexcept {
	glUseProgram( id_ );
	glUniform1i( GetUniform( name ), static_cast<int>( value ) );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetInt( const std::string& name, int value ) noexcept {
	glUseProgram( id_ );
	glUniform1i( GetUniform( name ), value );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetFloat( const std::string& name, float value ) noexcept {
	glUseProgram( id_ );
	glUniform1f( GetUniform( name ), value );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetMat3( const std::string& name, const glm::mat3& value,
						 bool transpose ) noexcept {
	glUseProgram( id_ );
	glUniformMatrix3fv( GetUniform( name ), 1, transpose,
						glm::value_ptr( value ) );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetMat4( const std::string& name, const glm::mat4& value,
						 bool transpose ) noexcept {
	glUseProgram( id_ );
	glUniformMatrix4fv( GetUniform( name ), 1, transpose,
						glm::value_ptr( value ) );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetVec3( const std::string& name,
						 const glm::vec3& value ) noexcept {
	glUseProgram( id_ );
	glUniform3fv( GetUniform( name ), 1, glm::value_ptr( value ) );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetVec3( const std::string& name, float x, float y,
						 float z ) noexcept {
	glUseProgram( id_ );
	glUniform3f( GetUniform( name ), x, y, z );
	glUseProgram( 0 );
	return *this;
}

Shader& Shader::SetTexture( const std::string& name, int index,
							const Texture& texture ) noexcept {
	assert( index >= 0 );

	glUseProgram( id_ );

	const auto uniform_name = name + "[" + to_string( index ) + "]";
	const auto location = GetUniform( uniform_name );
	glUniform1i( location, index );

	glActiveTexture( GL_TEXTURE0 + index );
	glBindTexture( GL_TEXTURE_2D, texture.id_ );

	glUseProgram( 0 );

	return *this;
}

void Shader::CompileFromFile( const std::filesystem::path& vertex_path,
							  const std::filesystem::path& fragment_path ) {
	auto load_shader = []( const std::filesystem::path& path ) -> std::string {
		auto filename = path.string();
		auto dir = Config::Instance().GetShaderDirectory().string();


		char* shader_directory = const_cast<char*>( dir.c_str() );
		char error[512];
		auto code = stb_include_file( const_cast<char*>( filename.c_str() ),
									  nullptr, shader_directory, error );

		if ( !code ) {
			throw Exception( "Shader pre-processing error: " +
							 string( error ) );
		}

		return code;
	};

	const auto vertex_code = load_shader( vertex_path );
	const auto fragment_code = load_shader( fragment_path );

	CompileFromString( vertex_code, fragment_code );
}

void Shader::CompileFromString( const std::string& vertex_source,
								const std::string& fragment_source ) {
	const char* v_shader_code = vertex_source.c_str();
	const char* f_shader_code = fragment_source.c_str();

	// 2. compile shaders
	GLuint vertex, fragment;
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

	id_ = glCreateProgram();
	glAttachShader( id_, vertex );
	glAttachShader( id_, fragment );
	glLinkProgram( id_ );
	// print linking errors if any
	glGetProgramiv( id_, GL_LINK_STATUS, &success );
	if ( !success ) {
		glGetProgramInfoLog( id_, 512, nullptr, info_log );
		throw CompileError( info_log );
	}
	// delete shaders; they�re linked into our program and no longer necessary
	glDeleteShader( vertex );
	glDeleteShader( fragment );
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

}  // namespace Octave
