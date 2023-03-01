#include "common/Sample.hpp"
#include <EntryPoint.hpp>

using namespace std;
using namespace Octave;

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

static glm::mat3 MakeNormalMatrix( const glm::mat4& model_matrix ) {
	return { glm::transpose( glm::inverse( model_matrix ) ) };
}

class ApplicationLayer : public Layer {
public:
	ApplicationLayer( Application& app, const string& file_name )
		: app_( app ) {
		renderer_ = app.GetGraphicsSystem().CreateRenderer();

		pad_ = app.GetInputSystem().GetGamepad( 0 );
		if ( pad_ != nullptr ) {
			cout << "Gamepad: " << pad_->GetName() << endl;
		}

		if ( Config::Instance().GetPreloadShaders() ) {
			shaders_.PreloadShaders();
		}

		shader_ = shaders_.Get( "basic" );
		if ( !shader_ ) {
			throw Exception( "Shader program not found" );
		}

		const auto [width, height] = app.GetWindow().GetSize();
		camera_.width_ = static_cast<float>( width );
		camera_.height_ = static_cast<float>( height );
		camera_.field_of_view_ = Config::Instance().GetFieldOfView();
		camera_.position_ = glm::vec3( 0, 0, 7 );

		// Load model file or basic cube if not provided
		if ( file_name.empty() ) {
			shared_ptr<VertexBuffer> vbo = app.GetGraphicsSystem().CreateVertexBuffer();
			shared_ptr<IndexBuffer> ibo = app.GetGraphicsSystem().CreateIndexBuffer();
			auto diffuse = make_shared<Texture>();

			GeometricPrimitive::CreateCube( *vbo, *ibo );
			diffuse->LoadFromFile( "./resources/textures/container.jpg" );

			Mesh cube_mesh;
			cube_mesh.SetVertexBuffer( vbo );
			cube_mesh.SetIndexBuffer( ibo );
			cube_mesh.SetTextures( { diffuse } );

			model_.AddMesh( std::move( cube_mesh ) );
		} else {
			model_ = Model::LoadFromFile( file_name );
		}
		model_matrix_ = glm::identity<glm::mat4>();

		// Load floor
		floor_vbo_ = app.GetGraphicsSystem().CreateVertexBuffer();
		floor_texture_diffuse_ = make_unique<Texture>();
		floor_texture_specular_ = make_unique<Texture>();

		GeometricPrimitive::CreatePlane( *floor_vbo_ );
		floor_texture_diffuse_->LoadFromFile(
			"./resources/textures/wood_diffuse.png" );
		floor_texture_specular_->LoadFromFile(
			"./resources/textures/wood_specular.png" );

		floor_position_ = glm::vec3( 0, -3, 0 );
	}

protected:
	void OnUpdate() override {
		step_timer_.Tick( [this]() {
			const auto delta =
				static_cast<float>( step_timer_.GetElapsedSeconds() );

			if ( app_.GetInputSystem().IsKeyDown( app_.GetWindow(),
												  Key::Escape ) ||
				 ( pad_ && pad_->IsButtonDown( GamepadButton::Select ) ) ) {
				app_.Exit();
			}

			DebugCameraControls( camera_, 25.0f, delta );

			if ( pad_ ) {
				DebugCameraControls( *pad_, camera_, 25.0f, delta );
			}

			model_matrix_ =
				glm::rotate( model_matrix_, glm::radians( delta * 25.0f ),
							 glm::vec3( 0, 1, 0 ) );
		} );

		renderer_->Clear( true, true, 0.1f, 0.1f, 0.1f );

		SetDefaultLighting( *shader_ );

		{
			shader_->SetMat4( "uMatProjection", camera_.GetProjectionMatrix() );
			shader_->SetMat4( "uMatView", camera_.GetViewMatrix() );
			shader_->SetMat4( "uMatModel", model_matrix_ );
			shader_->SetMat3( "uMatNormal", MakeNormalMatrix( model_matrix_ ) );

			shader_->SetVec3( "uViewPos", camera_.position_ );
			model_.Draw( *shader_, *renderer_ );
		}

		// Floor
		{
			shader_->SetTexture( "uTextures", 0, *floor_texture_diffuse_ );
			shader_->SetTexture( "uTextures", 1, *floor_texture_specular_ );

			auto model = glm::identity<glm::mat4>();
			model = glm::translate( model, floor_position_ );
			shader_->SetMat4( "uMatModel", model );
			shader_->SetMat3( "uMatNormal", MakeNormalMatrix( model ) );

			renderer_->Draw( *shader_, *floor_vbo_ );
		}
	}

	void DebugCameraControls( Octave::DebugCamera& camera, float camera_speed,
							  float delta ) noexcept {
		const auto& window = app_.GetWindow();
		const auto& keyboard = app_.GetInputSystem();

		// Strafe Left
		if ( keyboard.IsKeyDown( window, Key::A ) ) {
			camera.position_ -=
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
				camera_speed * delta;
		}
		// Strafe right
		if ( keyboard.IsKeyDown( window, Key::D ) ) {
			camera.position_ +=
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
				camera_speed * delta;
		}
		// Forward
		if ( keyboard.IsKeyDown( window, Key::W ) ) {
			camera.position_ += camera_speed * delta * camera.front_;
		}
		// Backward
		if ( keyboard.IsKeyDown( window, Key::S ) ) {
			camera.position_ -= camera_speed * delta * camera.front_;
		}
		// Up
		if ( keyboard.IsKeyDown( window, Key::E ) ) {
			camera.position_ += camera_speed * delta * camera.up_;
		}
		// Down
		if ( keyboard.IsKeyDown( window, Key::Q ) ) {
			camera.position_ -= camera_speed * delta * camera.up_;
		}

		// Turn left
		const float turn_speed = camera_speed * 3.0f;
		if ( keyboard.IsKeyDown( window, Key::Left ) ) {
			camera.yaw_ -= turn_speed * delta;
		}
		// Turn right
		if ( keyboard.IsKeyDown( window, Key::Right ) ) {
			camera.yaw_ += turn_speed * delta;
		}
		// Look up
		if ( keyboard.IsKeyDown( window, Key::Up ) ) {
			camera.pitch_ += turn_speed * delta;
		}
		// Look down
		if ( keyboard.IsKeyDown( window, Key::Down ) ) {
			camera.pitch_ -= turn_speed * delta;
		}
	}

	void DebugCameraControls( const Octave::Gamepad& gamepad,
							  Octave::DebugCamera& camera, float camera_speed,
							  float delta ) noexcept {
		const auto [left_x, left_y] = gamepad.GetLeftStick();

		// Move
		if ( left_x > 0.0f ) {
			camera.position_ +=
				left_x *
				glm::normalize( glm::cross( camera.front_, camera.up_ ) ) *
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

private:
	Application& app_;

	unique_ptr<Renderer> renderer_;
	std::shared_ptr<Octave::Shader> shader_;
	std::unique_ptr<Octave::Gamepad> pad_;

	Octave::ShaderManager shaders_;

	Octave::Model model_;
	glm::mat4 model_matrix_;

	StepTimer step_timer_;
	DebugCamera camera_;

	std::unique_ptr<Octave::VertexBuffer> floor_vbo_;
	std::unique_ptr<Octave::Texture> floor_texture_diffuse_,
		floor_texture_specular_;
	glm::vec3 floor_position_;
};

class ModelViewerSample : public Sample {
public:
	explicit ModelViewerSample( std::string file_name ) noexcept
		: file_name_( std::move( file_name ) ) {}

	void OnInitialize() override {
		PushLayer( make_unique<ApplicationLayer>( *this, file_name_ ) );
	}

private:
	std::string file_name_;
};

unique_ptr<Octave::Application> Octave::CreateApplication( int argc,
														   char* argv[] ) {
	string filename = argc > 1 ? argv[1] : "";
	return make_unique<ModelViewerSample>( filename );
}
