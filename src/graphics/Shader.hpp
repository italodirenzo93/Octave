#ifndef SHADER_HPP
#define SHADER_HPP

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

    void SetBool( const std::string& name, bool value ) const;
    void SetInt( const std::string& name, int value ) const;
    void SetFloat( const std::string& name, float value ) const;
    void SetMat4( const std::string& name, const glm::mat4& value ) const;
    void SetVec3( const std::string& name, const glm::vec3& value ) const;
    void SetVec3( const std::string& name, float x, float y, float z ) const;
    void SetTexture( const std::string& name, int index, const Texture& texture ) const;

private:
    uint32_t id_ = 0;

    Shader() = default;

public:
    Shader& operator=( Shader&& other ) noexcept;

private:
    static uint32_t CompileFromFile(const char* vertex_path, const char* fragment_path);

    NON_COPYABLE_CLASS( Shader )
};
}

#endif
