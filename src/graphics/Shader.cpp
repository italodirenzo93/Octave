#include "Shader.hpp"

#include <fstream>
#include <sstream>

static uint32_t compileFromFile( const char* vertexPath,
                                 const char* fragmentPath ) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try {
        // open files
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // read file�s buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (const std::ifstream::failure&) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    // vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if (!success) {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if (!success) {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    auto ID = glCreateProgram();
    glAttachShader( ID, vertex );
    glAttachShader( ID, fragment );
    glLinkProgram( ID );
    // print linking errors if any
    glGetProgramiv( ID, GL_LINK_STATUS, &success );
    if (!success) {
        glGetProgramInfoLog( ID, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
    }
    // delete shaders; they�re linked into our program and no longer necessary
    glDeleteShader( vertex );
    glDeleteShader( fragment );

    return ID;
}

namespace graphics {
Shader::Shader( const char* vertexPath, const char* fragmentPath ) {
    ID = compileFromFile( vertexPath, fragmentPath );
}

Shader::Shader( Shader&& other ) noexcept
    : ID( other.ID ) {
    other.ID = 0;
}

Shader::~Shader() {
    glDeleteProgram( ID );
}

void Shader::activate() const {
    glUseProgram( ID );
}

void Shader::deactivate() const {
    glUseProgram( 0 );
}

void Shader::setBool( const std::string& name, bool value ) const {
    glUniform1i( glGetUniformLocation( ID, name.c_str() ),
                 static_cast<int>(value) );
}

void Shader::setInt( const std::string& name, int value ) const {
    glUniform1i( glGetUniformLocation( ID, name.c_str() ), value );
}

void Shader::setFloat( const std::string& name, float value ) const {
    glUniform1f( glGetUniformLocation( ID, name.c_str() ), value );
}

void Shader::setMat4( const std::string& name, const glm::mat4& value ) const {
    auto location = glGetUniformLocation( ID, name.c_str() );
    glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( value ) );
}

void Shader::setVec3( const std::string& name, const glm::vec3& value ) const {
    auto location = glGetUniformLocation( ID, name.c_str() );
    glUniform3fv( location, 1, glm::value_ptr( value ) );
}

void Shader::setVec3( const std::string& name, float x, float y,
                      float z ) const {
    setVec3( name, glm::vec3( x, y, z ) );
}

Shader& Shader::operator=( Shader&& other ) noexcept {
    ID = other.ID;
    other.ID = 0;

    return *this;
}
}
