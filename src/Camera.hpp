#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "CommonInclude.hpp"
#include "graphics/Shader.hpp"

namespace octave {

class Camera {
public:
    Camera( float aspect_ratio, const glm::vec3& pos, const glm::vec3& up );
    ~Camera() = default;

    float field_of_view_ = 45.0f;
    float aspect_ratio_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float pitch_ = 0.0f;
    float yaw_ = -90.0f;

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    void Project( const graphics::Shader& shader ) const;
    void Unproject( const graphics::Shader& shader ) const;

    void Update();

private:
    NON_COPYABLE_OR_MOVABLE_CLASS( Camera )
};

}  // namespace octave

#endif
