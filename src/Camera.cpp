#include "Camera.hpp"

using namespace graphics;

Camera::Camera( float aspect_ratio, const glm::vec3& pos, const glm::vec3& up )
    : aspect_ratio_( aspect_ratio ), position_( pos ), up_( up ),
      world_up_( up ) {
    Update();
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective( glm::radians( field_of_view_ ), aspect_ratio_,
                             0.1f,
                             100.0f );
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt( position_, position_ + front_, up_ );
}

void Camera::Update() {
    front_ = glm::vec3( 1.0f );

    front_.x = cos( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );
    front_.y = sin( glm::radians( pitch_ ) );
    front_.z = sin( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );

    front_ = glm::normalize( front_ );
    right_ = glm::normalize( glm::cross( front_, world_up_ ) );
    up_ = glm::normalize( glm::cross( right_, front_ ) );
}

void Camera::Project( const Shader& shader ) const {
    shader.SetVec3( "uViewPos", position_ );
    shader.SetMat4( "uMatProjection", GetProjectionMatrix() );
    shader.SetMat4( "uMatView", GetViewMatrix() );
}

void Camera::Unproject( const Shader& shader ) const {
    shader.SetVec3( "uViewPos", glm::vec3( 0.0f ) );
    shader.SetMat4( "uMatProjection", glm::identity<glm::mat4>() );
    shader.SetMat4( "uMatView", glm::identity<glm::mat4>() );
}
