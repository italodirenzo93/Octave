#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "CommonInclude.hpp"
#include "graphics/Shader.hpp"

namespace octave {

class Camera {
public:
    Camera() noexcept;
    ~Camera() = default;

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const noexcept {
        return matrix_projection_;
    }
    [[nodiscard]] glm::mat4 GetViewMatrix() const noexcept {
        return matrix_view_;
    }
    [[nodiscard]] glm::vec3 GetPosition() const noexcept { return position_; };
    [[nodiscard]] float GetFieldOfView() const noexcept {
        return field_of_view_;
    }
    [[nodiscard]] float GetAspectRatio() const noexcept {
        return aspect_ratio_;
    }

    Camera& SetPosition( float x, float y, float z ) noexcept;
    Camera& SetPosition( const glm::vec3& position ) noexcept;
    Camera& SetFieldOfView( float fov ) noexcept;
    Camera& SetAspectRatio( float aspect ) noexcept;
    Camera& SetClipping( float near, float far ) noexcept;

    void Translate( float x = 0.0f, float y = 0.0f, float z = 0.0f ) noexcept;
    void Translate( const glm::vec3& translation ) noexcept;
    void Rotate( const glm::vec3& angles ) noexcept;

private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    const glm::vec3 world_up_ = glm::vec3( 0.0f, 1.0f, 0.0f );

    float field_of_view_;
    float aspect_ratio_;
    float clip_near_, clip_far_;

    float pitch_, yaw_, roll_;

    glm::mat4 matrix_projection_;
    glm::mat4 matrix_view_;

    void UpdateProjectionMatrix() noexcept;
    void UpdateViewMatrix() noexcept;

    NON_COPYABLE_OR_MOVABLE_CLASS( Camera )
};

}  // namespace octave

#endif
