#include "ModelViewerSample.hpp"

#include <memory>

#include "Config.hpp"
#include "GeometricPrimitive.hpp"
#include "graphics/ShaderManager.hpp"

using namespace std;

namespace octave::samples {

using namespace graphics;
using namespace input;

static void DebugCameraControls( const Keyboard& keyboard, DebugCamera& camera,
								 float camera_speed, float delta ) {
	// Strafe Left
	if ( keyboard.IsKeyDown( GLFW_KEY_A ) ) {
		camera.position_ -=
			glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
			camera_speed * delta;
	}
	// Strafe right
	if ( keyboard.IsKeyDown( GLFW_KEY_D ) ) {
		camera.position_ +=
			glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
			camera_speed * delta;
	}
	// Forward
	if ( keyboard.IsKeyDown( GLFW_KEY_W ) ) {
		camera.position_ += camera_speed * delta * camera.front_;
	}
	// Backward
	if ( keyboard.IsKeyDown( GLFW_KEY_S ) ) {
		camera.position_ -= camera_speed * delta * camera.front_;
	}
	// Up
	if ( keyboard.IsKeyDown( GLFW_KEY_E ) ) {
		camera.position_ += camera_speed * delta * camera.up_;
	}
	// Down
	if ( keyboard.IsKeyDown( GLFW_KEY_Q ) ) {
		camera.position_ -= camera_speed * delta * camera.up_;
	}

	// Turn left
	const float turn_speed = camera_speed * 3.0f;
	if ( keyboard.IsKeyDown( GLFW_KEY_LEFT ) ) {
		camera.yaw_ -= turn_speed * delta;
	}
	// Turn right
	if ( keyboard.IsKeyDown( GLFW_KEY_RIGHT ) ) {
		camera.yaw_ += turn_speed * delta;
	}
	// Look up
	if ( keyboard.IsKeyDown( GLFW_KEY_UP ) ) {
		camera.pitch_ += turn_speed * delta;
	}
	// Look down
	if ( keyboard.IsKeyDown( GLFW_KEY_DOWN ) ) {
		camera.pitch_ -= turn_speed * delta;
	}
}

static void DebugCameraControls( const Gamepad& gamepad, DebugCamera& camera,
								 float camera_speed, float delta ) {
	const auto [left_x, left_y] = gamepad.GetLeftStick();

	// Move
	if ( left_x > 0.0f ) {
		camera.position_ +=
			left_x * glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
			camera_speed * delta;
	}

	if ( left_y > 0.0f ) {
		camera.position_ += left_y * camera_speed * delta * camera.front_;
	}

	// Look
	const auto [right_x, right_y] = gamepad.GetRightStick();
	const float turn_speed = camera_speed * 3.0f;

	if ( right_x > 0.0f ) {
		camera.yaw_ += right_x * turn_speed * delta;
	}

	if ( right_y > 0.0f ) {
		camera.pitch_ += right_y * turn_speed * delta;
	}
}

inline void SetDefaultLighting( Shader& shader ) {
	const glm::vec3 direction( 0.5f, 0.0f, -0.5f );
	const glm::vec3 ambient( 0.2f );
	const glm::vec3 diffuse( 0.5f );
	const glm::vec3 specular( 0.8f );

	shader.SetVec3( "uDirectionalLight.direction", direction );
	shader.SetVec3( "uDirectionalLight.ambient", ambient );
	shader.SetVec3( "uDirectionalLight.diffuse", diffuse );
	shader.SetVec3( "uDirectionalLight.specular", specular );

	shader.SetBool( "uPointLights[0].enabled", true );
	shader.SetVec3( "uPointLights[0].position", glm::vec3( 2, 3, -2 ) );
	shader.SetVec3( "uPointLights[0].ambient", ambient );
	shader.SetVec3( "uPointLights[0].diffuse", diffuse );
	shader.SetVec3( "uPointLights[0].specular", specular );
	shader.SetFloat( "uPointLights[0].constant", 1.0f );
	shader.SetFloat( "uPointLights[0].linear", 0.09f );
	shader.SetFloat( "uPointLights[0].quadratic", 0.032f );
}

static inline glm::mat3 MakeNormalMatrix( const glm::mat4& model_matrix ) {
	return glm::mat3( glm::transpose( glm::inverse( model_matrix ) ) );
}

void ModelViewerSample::OnLoad() {
	cout << renderer_.GetDescription() << endl;

	if ( Gamepad::IsPresent( 0 ) ) {
		pad_ = make_unique<Gamepad>( 0 );
		cout << "Gamepad: " << pad_->GetName() << endl;
	}

	window_->AddSizeChangedCallback( [this]( int w, int h ) {
		if ( w > 0 && h > 0 ) {
			renderer_.SetViewport( 0, 0, w, h );
			camera_.width_ = static_cast<float>( w );
			camera_.height_ = static_cast<float>( h );
		}
	} );

	keyboard_ = make_unique<Keyboard>( *window_ );

	shader_ = ShaderManager::Instance().Get( "basic" );
	if ( !shader_ ) {
		throw Exception( "Shader program not found" );
	}

	const auto [width, height] = window_->GetSize();
	camera_.width_ = static_cast<float>( width );
	camera_.height_ = static_cast<float>( height );
	camera_.field_of_view_ = Config::Instance().GetFieldOfView();
	camera_.position_ = glm::vec3( 0, 0, 7 );

	model_ =
		Model::LoadFromFile( "./resources/objects/MemoryCard/MemoryCard.dae" );
	model_matrix_ = glm::identity<glm::mat4>();

	// Load floor
	GeometricPrimitive::CreatePlane( floor_vbo_ );
	floor_texture_diffuse_.LoadFromFile(
		"./resources/textures/wood_diffuse.png" );
	floor_texture_specular_.LoadFromFile(
		"./resources/textures/wood_specular.png" );

	floor_position_ = glm::vec3( 0, -3, 0 );
}

void ModelViewerSample::OnUnload() {
}

void ModelViewerSample::OnUpdate( const StepTimer& timer ) {
	const auto delta = static_cast<float>( timer.GetElapsedSeconds() );

	if ( keyboard_->IsKeyDown( GLFW_KEY_ESCAPE ) ||
		 ( pad_ && pad_->IsButtonDown( GLFW_GAMEPAD_BUTTON_BACK ) ) ) {
		Exit();
	}

	DebugCameraControls( *keyboard_, camera_, 25.0f, delta );

	if ( pad_ ) {
		DebugCameraControls( *pad_, camera_, 25.0f, delta );
	}

	model_matrix_ = glm::rotate( model_matrix_, glm::radians( delta * 25.0f ),
								 glm::vec3( 0, 1, 0 ) );
}

void ModelViewerSample::OnRender() {
	renderer_.Clear( true, true, 0.1f, 0.1f, 0.1f );

	renderer_.SetShader( *shader_ );
	SetDefaultLighting( *shader_ );

	{
		shader_->SetMat4( "uMatProjection", camera_.GetProjectionMatrix() );
		shader_->SetMat4( "uMatView", camera_.GetViewMatrix() );
		shader_->SetMat4( "uMatModel", model_matrix_ );
		shader_->SetMat3( "uMatNormal", MakeNormalMatrix( model_matrix_ ) );

		shader_->SetVec3( "uViewPos", camera_.position_ );
		model_.Draw( *shader_, renderer_ );
	}

	// Floor
	{
		shader_->SetTexture( "uTextures", 0, floor_texture_diffuse_ );
		shader_->SetTexture( "uTextures", 1, floor_texture_specular_ );

		auto model = glm::identity<glm::mat4>();
		model = glm::translate( model, floor_position_ );
		shader_->SetMat4( "uMatModel", model );
		shader_->SetMat3( "uMatNormal", MakeNormalMatrix( model ) );

		renderer_.DrawPrimitives( PrimitiveType::kTriangleList, floor_vbo_ );
	}

	renderer_.Present();
}

}  // namespace octave::samples
