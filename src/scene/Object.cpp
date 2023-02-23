#include "Object.hpp"

using namespace glm;

namespace octave::scene {

Object::Object() noexcept {
	model_matrix_ = identity<mat4>();
	position_ = zero<vec3>();
	euler_ = zero<vec3>();
	scale_ = one<vec3>();
}

mat4 Object::GetModelMatrix() noexcept {
	if ( update_model_matrix_ ) {
		model_matrix_ = translate( mat4( 1.0f ), position_ ) *
						mat4_cast( quat( euler_ ) ) *
						scale( mat4( 1.0f ), scale_ );
		update_model_matrix_ = false;
	}

	return model_matrix_;
}

Object& Object::SetPosition( const vec3& position ) noexcept {
	position_ = position;
	update_model_matrix_ = true;
	return *this;
}

Object& Object::Translate( const vec3& translation ) noexcept {
	position_ += translation;
	model_matrix_ = translate( model_matrix_, translation );
	return *this;
}

}  // namespace octave::scene
