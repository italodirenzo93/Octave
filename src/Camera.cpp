#include "Camera.hpp"

namespace octave {

using namespace graphics;

Camera::Camera() noexcept {
	field_of_view_ = 45.0f;
	aspect_ratio_ = 16.0f / 9.0f;
	position_ = glm::vec3( 0.0f );
	clip_near_ = 0.1f;
	clip_far_ = 100.0f;

	front_ = glm::vec3( 0.0f, 0.0f, -1.0f );
	up_ = world_up_;
	right_ = glm::vec3( 1.0f, 0.0f, 0.0f );

	yaw_ = -90.0f;
	pitch_ = 0.0f;

	matrix_projection_ = glm::identity<glm::mat4>();
	matrix_view_ = glm::identity<glm::mat4>();

	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

Camera& Camera::SetPosition( float x, float y, float z ) noexcept {
	position_ = glm::vec3( x, y, z );
	UpdateViewMatrix();
	return *this;
}

Camera& Camera::SetPosition( const glm::vec3& position ) noexcept {
	position_ = position;
	UpdateViewMatrix();
	return *this;
}

Camera& Camera::SetFieldOfView( float fov ) noexcept {
	field_of_view_ = fov;
	UpdateProjectionMatrix();
	return *this;
}

Camera& Camera::SetAspectRatio( float aspect ) noexcept {
	aspect_ratio_ = aspect;
	UpdateProjectionMatrix();
	return *this;
}

Camera& Camera::SetClipping( float near, float far ) noexcept {
	clip_near_ = near;
	clip_far_ = far;
	UpdateProjectionMatrix();
	return *this;
}

void Camera::Translate( float x, float y, float z ) noexcept {
	Translate( glm::vec3( x, y, z ) );
}

void Camera::Translate( const glm::vec3& translation ) noexcept {
	if ( translation != glm::zero<glm::vec3>() ) {
		// Forward / Backward
		position_ += translation * front_;

		// Side to side
		position_ += glm::normalize( glm::cross( front_, up_ ) ) * translation;

		UpdateViewMatrix();
	}
}

void Camera::Rotate( const glm::vec3& angles ) noexcept {
	if ( angles != glm::zero<glm::vec3>() ) {
		// Look left/right
		yaw_ += angles.y;

		// Look up/down
		pitch_ = glm::clamp( pitch_ + angles.x, -89.0f, 89.0f );

		UpdateViewMatrix();
	}
}

void Camera::UpdateProjectionMatrix() noexcept {
	matrix_projection_ = glm::perspective(
		glm::radians( field_of_view_ ), aspect_ratio_, clip_near_, clip_far_ );
}

void Camera::UpdateViewMatrix() noexcept {
	front_ = glm::vec3( 1.0f );

	front_.x = cos( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );
	front_.y = sin( glm::radians( pitch_ ) );
	front_.z = sin( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );

	front_ = glm::normalize( front_ );
	right_ = glm::normalize( glm::cross( front_, world_up_ ) );
	up_ = glm::normalize( glm::cross( right_, front_ ) );

	matrix_view_ = glm::lookAt( position_, position_ + front_, up_ );
}

}  // namespace octave
