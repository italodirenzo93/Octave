#include "common/Sample.hpp"
#include <EntryPoint.hpp>

#include "helpers/GeometricPrimitive.hpp"

using namespace std;
using namespace Octave;
using namespace Octave::Samples;

struct Matrices {
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 model_matrix;
	glm::mat3 normal_matrix;
};

inline void SetDefaultLighting( Shader& shader ) {
	const glm::vec3 direction( 0.5f, 0.0f, -0.5f );
	const glm::vec3 ambient( 0.2f );
	const glm::vec3 diffuse( 0.5f );
	const glm::vec3 specular( 0.8f );

	//shader.SetVec3( "uDirectionalLight.direction", direction );
	//shader.SetVec3( "uDirectionalLight.ambient", ambient );
	//shader.SetVec3( "uDirectionalLight.diffuse", diffuse );
	//shader.SetVec3( "uDirectionalLight.specular", specular );

	//shader.SetBool( "uPointLights[0].enabled", true );
	//shader.SetVec3( "uPointLights[0].position", glm::vec3( 2, 3, -2 ) );
	//shader.SetVec3( "uPointLights[0].ambient", ambient );
	//shader.SetVec3( "uPointLights[0].diffuse", diffuse );
	//shader.SetVec3( "uPointLights[0].specular", specular );
	//shader.SetFloat( "uPointLights[0].constant", 1.0f );
	//shader.SetFloat( "uPointLights[0].linear", 0.09f );
	//shader.SetFloat( "uPointLights[0].quadratic", 0.032f );
}

static glm::mat3 MakeNormalMatrix( const glm::mat4& model_matrix ) {
	return { glm::transpose( glm::inverse( model_matrix ) ) };
}

class ApplicationLayer : public Layer {
public:
	explicit ApplicationLayer( Application& app )
		: app_( app ) {

		context_ = app.GetGraphicsDevice().CreateContext();

		pad_ = app.GetInputSystem().GetGamepad( 0 );
		if ( pad_ != nullptr ) {
			cout << "Gamepad: " << pad_->GetName() << endl;
		}

		const auto [width, height] = app.GetWindow().GetSize();
		camera_.width_ = static_cast<float>( width );
		camera_.height_ = static_cast<float>( height );
		camera_.field_of_view_ = Config::Instance().GetFieldOfView();
		camera_.position_ = glm::vec3( 0, 0, 7 );

		// Load GPU resources for objects
		{
			GeometricPrimitive::VertexCollection vertices;
			GeometricPrimitive::IndexCollection indices;

			GeometricPrimitive::CreateCube( vertices, indices );

			// Create cube GPU resources
			cube_vbo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), vertices,
									sizeof( GeometricPrimitive::VertexType ),
									BufferBinding::VertexBuffer );

			std::array<VertexAttribute, 3> attrs{
				VertexAttribute{ VertexAttributeName::kPosition, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kNormal, 3,
								 VertexAttributeType::kFloat, false },
				VertexAttribute{ VertexAttributeName::kTexCoord, 2,
								 VertexAttributeType::kFloat, false } };

			VertexArrayDescription vao_desc{};
			vao_desc.attributes = attrs.data();
			vao_desc.count = attrs.size();

			cube_vao_ = app.GetGraphicsDevice().CreateVertexArray( vao_desc );

			cube_ibo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), indices, 0,
									BufferBinding::IndexBuffer );

			cube_texture_ = CreateTextureFromFile(
				app.GetGraphicsDevice(), "resources/textures/container.jpg" );
		}

		// Load floor
		{
			GeometricPrimitive::VertexCollection floor_verts;
			GeometricPrimitive::CreatePlane( floor_verts );

			floor_vbo_ =
				CreateStaticBuffer( app.GetGraphicsDevice(), floor_verts,
									sizeof( GeometricPrimitive::VertexType ),
									BufferBinding::VertexBuffer );
			floor_texture_diffuse_ =
				CreateTextureFromFile( app.GetGraphicsDevice(),
									   "resources/textures/wood_diffuse.png" );
			floor_texture_specular_ =
				CreateTextureFromFile( app.GetGraphicsDevice(),
									   "resources/textures/wood_specular.png" );

			floor_position_ = glm::vec3( 0, -3, 0 );
		}

		// Uniform buffer
		{
			BufferDescription desc{};
			desc.size = sizeof( Matrices );
			desc.access_flags = ResourceAccess::Write;
			desc.bind_flags = BufferBinding::UniformBuffer;

			Matrices matrices{};
			matrices.projection_matrix = camera_.GetProjectionMatrix();
			matrices.view_matrix = camera_.GetViewMatrix();
			matrices.model_matrix = glm::identity<glm::mat4>();
			matrices.normal_matrix = glm::identity<glm::mat3>();

			uniform_buffer_ =
				app.GetGraphicsDevice().CreateBuffer( desc, &matrices );
		}

		// Shaders
		{
			vertex_shader_ = LoadShaderFromFile(
				app.GetGraphicsDevice(), ShaderType::VertexShader,
				"resources/shaders/basic.vert" );

			fragment_shader_ = LoadShaderFromFile(
				app.GetGraphicsDevice(), ShaderType::FragmentShader,
				"resources/shaders/basic.frag" );

			pipeline_ = app.GetGraphicsDevice().CreatePipeline();

			pipeline_->SetVertexShader( vertex_shader_ );
			pipeline_->SetVertexUniformBuffer( uniform_buffer_, 0, 0 );

			pipeline_->SetFragmentShader( fragment_shader_ );
		}
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

		context_->Clear( true, true, 0.1f, 0.1f, 0.1f );

		context_->SetVertexBuffer( cube_vbo_, cube_vao_ );
		context_->SetIndexBuffer( cube_ibo_ );
		context_->SetPipeline( pipeline_ );
		context_->DrawIndexed( cube_ibo_->GetSize(), 0, 0 );

		//SetDefaultLighting( *shader_ );

		// Floor
		//{
		//	shader_->SetTexture( "uTextures", 0, *floor_texture_diffuse_ );
		//	shader_->SetTexture( "uTextures", 1, *floor_texture_specular_ );

		//	auto model = glm::identity<glm::mat4>();
		//	model = glm::translate( model, floor_position_ );
		//	shader_->SetMat4( "uMatModel", model );
		//	shader_->SetMat3( "uMatNormal", MakeNormalMatrix( model ) );

		//	renderer_->Draw( *shader_, *floor_vbo_ );
		//}
	}

	void DebugCameraControls( DebugCamera& camera, float camera_speed,
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
							  DebugCamera& camera, float camera_speed,
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

	Ref<GraphicsContext> context_;

	SharedRef<Buffer> uniform_buffer_;
	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
	SharedRef<Pipeline> pipeline_;

	Ref<Gamepad> pad_;

	//ShaderManager shaders_;

	glm::mat4 model_matrix_;

	StepTimer step_timer_;
	DebugCamera camera_;

	SharedRef<Buffer> cube_vbo_;
	SharedRef<VertexArray> cube_vao_;
	SharedRef<Buffer> cube_ibo_;
	SharedRef<Texture> cube_texture_;

	SharedRef<Buffer> floor_vbo_;
	SharedRef<Texture> floor_texture_diffuse_,
		floor_texture_specular_;
	glm::vec3 floor_position_;

};

class ModelViewerSample : public Sample {
public:
	explicit ModelViewerSample() noexcept {}

	void OnInitialize() override {
		PushLayer( MakeRef<ApplicationLayer>( *this ) );
	}
};

Ref<Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return MakeRef<ModelViewerSample>();
}
