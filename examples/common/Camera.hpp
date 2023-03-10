#ifndef OCTAVE_SAMPLES_CAMERA_HPP
#define OCTAVE_SAMPLES_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Octave::Samples {

//class Camera {
//public:
//	Camera() noexcept;
//	~Camera() noexcept = default;
//
//	[[nodiscard]] glm::mat4 GetProjectionMatrix() const noexcept {
//		return matrix_projection_;
//	}
//	[[nodiscard]] glm::mat4 GetViewMatrix() const noexcept {
//		return matrix_view_;
//	}
//	[[nodiscard]] glm::vec3 GetPosition() const noexcept { return position_; };
//	[[nodiscard]] float GetFieldOfView() const noexcept {
//		return field_of_view_;
//	}
//	[[nodiscard]] float GetAspectRatio() const noexcept {
//		return aspect_ratio_;
//	}
//
//	Camera& SetPosition( float x, float y, float z ) noexcept;
//	Camera& SetPosition( const glm::vec3& position ) noexcept;
//	Camera& SetFieldOfView( float fov ) noexcept;
//	Camera& SetAspectRatio( float aspect ) noexcept;
//	Camera& SetClipping( float near, float far ) noexcept;
//
//	void Translate( float x = 0.0f, float y = 0.0f, float z = 0.0f ) noexcept;
//	void Translate( const glm::vec3& translation ) noexcept;
//	void Rotate( const glm::vec3& angles ) noexcept;
//
//private:
//	glm::vec3 position_;
//	glm::vec3 front_;
//	glm::vec3 up_;
//	glm::vec3 right_;
//	const glm::vec3 world_up_ = glm::vec3( 0.0f, 1.0f, 0.0f );
//
//	float field_of_view_;
//	float aspect_ratio_;
//	float clip_near_, clip_far_;
//
//	float pitch_, yaw_;
//
//	glm::mat4 matrix_projection_;
//	glm::mat4 matrix_view_;
//
//	void UpdateProjectionMatrix() noexcept;
//	void UpdateViewMatrix() noexcept;
//
//public:
//	Camera( const Camera& ) = delete;
//	Camera( Camera&& ) = delete;
//	Camera& operator=( const Camera& ) = delete;
//	Camera& operator=( Camera&& ) = delete;
//};


class DebugCamera {
public:
	// Projection variables
	float field_of_view_ = 45.0f;
	float width_ = 0.0f;
	float height_ = 0.0f;
	float clip_near_ = 0.1f;
	float clip_far_ = 100.0f;

	// View variables
	glm::vec3 position_ = glm::vec3( 0, 0, 3 );
	glm::vec3 front_ = glm::vec3( 0, 0, -1 );
	glm::vec3 up_ = glm::vec3( 0, 1, 0 );
	float yaw_ = -90;
	float pitch_ = 0.0f;

	[[nodiscard]] glm::mat4 GetProjectionMatrix() const noexcept {
		return glm::perspectiveFov( glm::radians( field_of_view_ ), width_,
									height_, clip_near_, clip_far_ );
	}

	[[nodiscard]] glm::mat4 GetViewMatrix() noexcept {
		pitch_ = glm::clamp( pitch_, -89.0f, 89.0f );

		glm::vec3 front( 0.0f );

		front.x = cos( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );
		front.y = sin( glm::radians( pitch_ ) );
		front.z = sin( glm::radians( yaw_ ) ) * cos( glm::radians( pitch_ ) );

		front_ = glm::normalize( front );

		return glm::lookAt( position_, position_ + front_, up_ );
	}
};


}  // namespace octave

#endif
