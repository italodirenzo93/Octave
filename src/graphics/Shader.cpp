#include "Shader.hpp"

#include <fstream>
#include <sstream>

using namespace std;

namespace graphics {

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
    } catch (const std::ifstream::failure&) {
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
    if (!success) {
        glGetShaderInfoLog( vertex, 512, nullptr, info_log );
        throw CompileError( info_log );
    }

    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &f_shader_code, nullptr );
    glCompileShader( fragment );
    // print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if (!success) {
        throw CompileError( info_log );
    }

    auto id = glCreateProgram();
    glAttachShader( id, vertex );
    glAttachShader( id, fragment );
    glLinkProgram( id );
    // print linking errors if any
    glGetProgramiv( id, GL_LINK_STATUS, &success );
    if (!success) {
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

Shader::Shader( Shader&& other ) noexcept
    : id_( other.id_ ) {
    other.id_ = 0;
}

Shader::~Shader() noexcept {
    glDeleteProgram( id_ );
}

void Shader::SetBool( const std::string& name, bool value ) const {
    glUniform1i( glGetUniformLocation( id_, name.c_str() ),
                 static_cast<int>(value) );
}

void Shader::SetInt( const std::string& name, int value ) const {
    glUniform1i( glGetUniformLocation( id_, name.c_str() ), value );
}

void Shader::SetFloat( const std::string& name, float value ) const {
    glUniform1f( glGetUniformLocation( id_, name.c_str() ), value );
}

void Shader::SetMat4( const std::string& name, const glm::mat4& value ) const {
    auto location = glGetUniformLocation( id_, name.c_str() );
    glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( value ) );
}

void Shader::SetVec3( const std::string& name, const glm::vec3& value ) const {
    auto location = glGetUniformLocation( id_, name.c_str() );
    glUniform3fv( location, 1, glm::value_ptr( value ) );
}

void Shader::SetVec3( const std::string& name, float x, float y,
                      float z ) const {
    SetVec3( name, glm::vec3( x, y, z ) );
}

Shader& Shader::operator=( Shader&& other ) noexcept {
    id_ = other.id_;
    other.id_ = 0;

    return *this;
}

}
