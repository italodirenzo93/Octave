#ifndef SHADER_HPP
#define SHADER_HPP

#include "CommonInclude.hpp"

namespace graphics {
class Shader {
public:
    explicit Shader( const char* vertex_path, const char* fragment_path );
    Shader( Shader&& other ) noexcept;
    ~Shader();

    uint32_t GetId() const { return id_; }

    void Activate() const;
    void Deactivate() const;
    void SetBool( const std::string& name, bool value ) const;
    void SetInt( const std::string& name, int value ) const;
    void SetFloat( const std::string& name, float value ) const;
    void SetMat4( const std::string& name, const glm::mat4& value ) const;
    void SetVec3( const std::string& name, const glm::vec3& value ) const;
    void SetVec3( const std::string& name, float x, float y, float z ) const;

private:
    uint32_t id_ = 0;

    explicit Shader( uint32_t id )
        : id_( id ) {
    }

    Shader() = default;

public:
    Shader& operator=( Shader&& other ) noexcept;

private:
    NON_COPYABLE_CLASS( Shader )
};
}

#endif
